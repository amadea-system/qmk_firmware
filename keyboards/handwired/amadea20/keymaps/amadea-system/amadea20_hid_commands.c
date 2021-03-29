/* 
 * Implementation for Hid Command Protocal
 * 
 * Copyright 2021 Amadea System
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


#include "quantum.h"
#include "raw_hid.h"
#include "amadea20_hid_commands.h"
#include "amadea20_common.h"


/* ------------ # Defines ------------ */

// - HID -
#define HID_DISCONECTION_TIMEOUT 5000  // milliseconds 


// -- oled --
// #define OLED_CHAR_WIDTH ((uint8_t)(OLED_DISPLAY_WIDTH / OLED_FONT_WIDTH))


/* --------- Extern Variables -------- */

// OLED Vars
extern uint32_t last_led_changed_time;

extern uint8_t current_fronter;

/* --------- Local Variables --------- */

// HID Vars
bool hid_connected = false;         // Flag indicating if we have a PC connection yet
// bool send_activity_ping = false;    // Flag indicating that an activity ping should be sent at the next avalible time.

// static uint16_t hid_disconection_timer = 0;
// static uint16_t activity_ping_timer = 0;
// static uint16_t last_hid_transmition_time = 0;


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


// void queue_activity_ping(void){
//     send_activity_ping = true;
// }

// void send_queued_activity_ping(void){

//     if (!send_activity_ping || !hid_connected || timer_elapsed(activity_ping_timer) < 5000 || timer_elapsed(last_hid_transmition_time) < 100) {
//         // Only send a ping once every 5 seconds and only if we are connected.
//         return;
//     }
//     send_activity_ping = false;
//     activity_ping_timer = timer_read();  // Reset the activity ping timer
//     uint8_t send_data[32] = {0};  // data packet must be 32 bytes on 8bit AVR platform
//     send_data[0] = CMD_PC_ACTIVITY_PING;
//     raw_hid_send(send_data, sizeof(send_data));
// }

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
    
    // last_hid_transmition_time = timer_read();  // Reset the hid transmition timer
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
    // hid_disconection_timer = timer_read();  // Reset the timeout timer

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

            default:
                // We either recived a not yet supported HID call or a malformed packet. Do nothing.
                break;
        }
    }
}

// All hid Timer code clocks in at approx 47 bytes
bool check_hid_timeout(void){

    // if (!hid_connected || timer_elapsed(hid_disconection_timer) > HID_DISCONECTION_TIMEOUT) {
    //     hid_connected = false;
    //     return false;
    // }
    return true;
}

#endif // -HID RAW_ENABLE 
// --------------
