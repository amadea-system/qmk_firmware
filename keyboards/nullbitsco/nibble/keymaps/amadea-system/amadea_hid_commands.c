/* 
 * Implementation for Hid Command Protocal
 * 
 * Copyright 2020 Amadea System
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "amadea_hid_commands.h"

#include "quantum.h"
#include "raw_hid.h"

#include "nibble65_keymap.h"


/* ------------ # Defines ------------ */

// - HID -
#define HID_DISCONECTION_TIMEOUT 5000  // milliseconds 

// -- oled --
// #define OLED_CHAR_WIDTH ((uint8_t)(OLED_DISPLAY_WIDTH / OLED_FONT_WIDTH))


/* --------- Extern Variables -------- */

// OLED Vars
extern uint32_t last_led_changed_time;
extern uint16_t volume_changed_timer;
extern bool show_volume_display;

extern uint8_t last_volume_set_point;
extern uint8_t last_volume_target;
extern char last_volume_app_name[OLED_CHAR_WIDTH+1];

extern uint8_t current_fronter;

/* --------- Local Variables --------- */

// HID Vars
bool hid_connected = false;         // Flag indicating if we have a PC connection yet
bool send_activity_ping = false;    // Flag indicating that an activity ping should be sent at the next avalible time.
bool send_volume_change = false;    // Flag indicating that a volume change should be sent at the next avalible time.

static uint16_t hid_disconection_timer = 0;
static uint16_t activity_ping_timer = 0;
static uint16_t last_hid_transmition_time = 0;

static uint8_t next_volume_change_target = 255;
static int8_t next_volume_change_amount = 0;


/* ---------- Function Defs ---------- */


void set_rgblight_current_fronter(uint8_t fronter);
void set_rgblight_from_pc_cmd(uint8_t *led_data, uint8_t length);
void send_hid_debug(uint8_t *data);

void raw_hid_receive(uint8_t *data, uint8_t length);


/* ------------ Functions ------------ */

#ifdef RAW_ENABLE

#ifdef RGBLIGHT_ENABLE
void set_rgblight_current_fronter(uint8_t fronter){
    switch (fronter) {
        case MEM_SWITCHED_OUT:
            rgblight_sethsv_noeeprom(HSV_WHITE);
            break;
        case MEM_FLUTTERSHY:
            rgblight_sethsv_noeeprom(HSV_YELLOW);
            break;
        case MEM_HIBIKI:
            rgblight_sethsv_noeeprom(HSV_PURPLE);
            break;
        case MEM_LUNA:
            // rgblight_sethsv_noeeprom(HSV_BLUE);
            rgblight_sethsv_noeeprom(240, 255, 255);  // Pink
            break;
        default:
            rgblight_sethsv_noeeprom(HSV_RED);
            break;
    }

    last_led_changed_time = timer_read32(); 
}

// https://github.com/qmk/qmk_firmware/blob/master/quantum/rgblight.h
// Sets each LED to the corresponding HSV value sent from the PC
void set_rgblight_from_pc_cmd(uint8_t *led_data, uint8_t length){  // 128 Bytes.
    /* 
    * Data Format:
    * Byte n+0: LED Number
    * Byte n+1: Hue
    * Byte n+2: Saturation
    * Byte n+3: Value
    * */
    if (!rgblight_is_enabled()) {
         return;
    }
    uint8_t i;
    for (i=0; i < length; i+=4)
    {
        sethsv(led_data[i+1], led_data[i+2], led_data[i+3], (LED_TYPE *) &led[led_data[i]]);  // Hue, Sat, Val, LED Num
    }
    rgblight_set();

    last_led_changed_time = timer_read32(); 
}
#endif  // -RGBLIGHT_ENABLE


void send_hid_debug(uint8_t *data){
    raw_hid_send_command(CMD_PC_RAW_DEBUG_MSG, data, sizeof(data));
}


void queue_volume_change(int8_t volume_amount, uint8_t volume_target){
    send_volume_change = true;

    if(next_volume_change_target == 255){
        next_volume_change_target = volume_target;
    }

    // if(next_volume_change_amount > 110 ){
    //     next_volume_change_amount = volume_amount;
    // }else{
    //     next_volume_change_amount += volume_amount;
    // }
    next_volume_change_amount += volume_amount;
}


void send_queued_incremental_volume_change(void){

    if (!send_volume_change || !hid_connected || timer_elapsed(last_hid_transmition_time) < 500) {
        // Only send volume changes once every 0.5 seconds.
        return;
    }

    if(next_volume_change_amount > 0){
        send_hid_cmd_change_volume(VOLUME_UP_INC, (uint8_t)next_volume_change_amount, next_volume_change_target);
    }else if(next_volume_change_amount < 0){
        send_hid_cmd_change_volume(VOLUME_DOWN_INC, (uint8_t)(next_volume_change_amount*-1), next_volume_change_target);
    }

    next_volume_change_amount = 0;
    next_volume_change_target = 255;
    send_volume_change = false;
}


// Send a hid command to the PC to change the volume.
// volume_direction - 0: Lower Volume, 1: Raise Volume, 2: Mute/Unmute Volume, 3: Set Volume Absolute
// target - 0: Change Volume in the Focused App, 1: Change Volume System Wide, 2: Change Volume for a specific process.
void send_hid_cmd_change_volume(uint8_t volume_direction, uint8_t volume_amount, uint8_t target){
    /* 
    * Data Format:
    * Byte 0: Volume Direction (0: Down Incremental, 1: Up Incremental, 2: Mute/Unmute, 3: Set Volume Absolute)
    * Byte 1: Volume Change/Set Amount. Out of 100. Ignored for Mute Commands.
    * Byte 2: App Target: Focused App (0), System Wide (1), Specific Process (2) [Future...]
    * Byte n+?: (Future) Some sort of ID for the specific process.
    * */

    uint8_t payload_data[5] = {0}; 
    payload_data[0] = volume_direction; 
    payload_data[1] = volume_amount;
    payload_data[2] = target;

    raw_hid_send_command(CMD_PC_CHANGE_VOLUME, payload_data, sizeof(payload_data));

}

// Selects the Next/Previous/None Process to change the volume of.
void send_hid_cmd_get_next_volume_process(uint8_t search_direction){
    /* 
    * Data Format:
    * Byte 0: Search Direction (0: Previous Process, 1: Next Process, 2: Current Process)
    * */
    uint8_t payload_data[1] = {0}; 
    payload_data[0] = search_direction;
    raw_hid_send_command(CMD_PC_GET_VOL_PROCESS, payload_data, sizeof(payload_data));

}


void queue_activity_ping(void){
    send_activity_ping = true;
}

void send_queued_activity_ping(void){

    if (!send_activity_ping || !hid_connected || timer_elapsed(activity_ping_timer) < 5000 || timer_elapsed(last_hid_transmition_time) < 100) {
        // Only send a ping once every 5 seconds and only if we are connected.
        return;
    }
    send_activity_ping = false;
    activity_ping_timer = timer_read();  // Reset the activity ping timer
    uint8_t send_data[32] = {0};  // data packet must be 32 bytes on 8bit AVR platform
    send_data[0] = CMD_PC_ACTIVITY_PING;
    raw_hid_send(send_data, sizeof(send_data));
}

void raw_hid_send_command(uint8_t command_id, uint8_t *data, uint8_t length) {
/*
* Sent HID Data Packet Format:
* Byte 0: The Command Type
* Byte 1: Length of Command Data
* Byte 2-31: Command Data 
*
*/

    // HID Packet is 32 bytes. Payload data gets 30 bytes of that.
    uint8_t safe_length = length;
    if (safe_length > 30)
    {
        safe_length = 30;
    }
    
    last_hid_transmition_time = timer_read();  // Reset the hid transmition timer
    uint8_t send_data[32] = {0};  // data packet must be 32 bytes on 8bit AVR platform
    send_data[0] = command_id;
    send_data[1] = safe_length;
    memcpy(&send_data[2], data, safe_length);
    raw_hid_send(send_data, sizeof(send_data));

}

void raw_hid_receive(uint8_t *data, uint8_t length) {
/*
* Received HID Data Packet Format:
* Byte 0: The Command Type
* Byte 1: Length of Command Data
* Byte 2-31: Command Data 
*
* NOTE: The first byte sent from the PC (Report ID) does not equal the first byte here. The Report ID is not included here.
*/

    hid_connected = true;       // PC connected
    hid_disconection_timer = timer_read();  // Reset the timeout timer

    if (length >= 3) {
        uint8_t *command_id   = &(data[0]);
        uint8_t *data_length  = &(data[1]); 
        uint8_t *command_data = &(data[2]);

        switch(*command_id){
            case CMD_DO_NOTHING:  // Exists so we can keep the disconection timer alive.
                break;
            case CMD_KB_SET_CURRENT_FRONTER:
                if(current_fronter != command_data[0]){
                    current_fronter = command_data[0];
                    #ifdef RGBLIGHT_ENABLE
                    set_rgblight_current_fronter(current_fronter);
                    #endif
                }
                break;

            case CMD_KB_SET_RGB_LEDS:
                #ifdef RGBLIGHT_ENABLE
                if(*data_length % 4 == 0){  // Data Len must be a multiple of 4 for this function.
                    set_rgblight_from_pc_cmd(command_data, *data_length);
                }else{
                    ; // TODO: Handle This error case
                }
                #endif
                break;

            case CMD_KB_SHOW_CURRENT_VOLUME:
            {
                /*
                * Payload Packet Format:
                * Byte 0:          Volume Pct Or Mute. Pct is 0-100, Mute is 255.
                * Byte 1:          App Target: Focused App (0), System Wide (1), Specific Process (2) [Future...]
                * Byte 2:          App Name Length
                * Byte 3+n (char): The name of the App (21 max len)
                */
                
                last_volume_set_point = command_data[0];
                last_volume_target = command_data[1];
                uint8_t app_name_len = command_data[2];
                memcpy(last_volume_app_name, &command_data[3], app_name_len);
                memset(&last_volume_app_name[app_name_len], ' ', sizeof(last_volume_app_name)-app_name_len-1);

                // raw_hid_send_command(CMD_PC_RAW_DEBUG_MSG,  (unsigned char *) last_volume_app_name, app_name_len);
                volume_changed_timer = timer_read();
                show_volume_display = true;
                // send_hid_debug( (unsigned char *) last_volume_app_name);
                // send_hid_debug(command_data);
            break;
            }

            default:
                // We either recived a not yet supported HID call or a malformed packet. Do nothing.
                break;
        }
    }
}

// All hid Timer code clocks in at approx 47 bytes
bool check_hid_timeout(void){

    if (!hid_connected || timer_elapsed(hid_disconection_timer) > HID_DISCONECTION_TIMEOUT) {
        hid_connected = false;
        return false;
    }
    return true;
}

#endif // -HID RAW_ENABLE 
// --------------
