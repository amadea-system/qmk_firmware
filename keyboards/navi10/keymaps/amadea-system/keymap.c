/* Copyright 2019 Ethan Durrant (emdarcher)
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
#include QMK_KEYBOARD_H
#include "print.h"
#include "raw_hid.h"

#include "string.h" // For memcpy

#include "navi10_keymap_enums.h"


// Default Backlight Color
#define DEFAULT_ARGB_COLOR HSV_SPRINGGREEN  //HSV_PURPLE
#define MEM_SWITCH_OUT_COLOR HSV_BLUE

//create the tap type
typedef struct {
    bool is_press_action;
    int state;
} tap;


typedef struct {
    uint8_t hue;
    uint8_t sat;
}fav_color;


// static fav_color fav_colors[4] = {
//     { 0, 0 },       // Switched out. WHITE
//     { 43, 255 },    // Fluttershy. YELLOW
//     { 191, 255 },   // Hibiki. PURPLE
//     { 170, 255 }    // Luna. BLUE
// };
// struct fav_color fav_colors[4];
 
// fav_colors[MEM_SWITCHED_OUT] = struct fav_color { 0, 0 }; // Switched out. WHITE
// fav_colors[MEM_FLUTTERSHY] = fav_color { 43, 255 };    // Fluttershy. YELLOW
// fav_colors[MEM_HIBIKI] = fav_color { 191, 255 };   // Hibiki. PURPLE
// fav_colors[MEM_LUNA] = fav_color { 170, 255 };   // Luna. BLUE

typedef struct {
    uint8_t start;
    uint8_t end;
}fronter_led_range_t;


fronter_led_range_t fronter_led_range = {1, 8};
HSV fronter_hsv_value;


// HID Vars
bool hid_connected = false; // Flag indicating if we have a PC connection yet
uint8_t current_fronter = MEM_SWITCHED_OUT;
// static uint16_t hid_disconection_timer;

// --- Function Defs ---
// - HID Funcs - 
// void send_hid_debug(uint8_t *data);
void raw_hid_send_command(uint8_t command_id, uint8_t *data, uint8_t length);

// - RGB LED Funcs - 
// void set_hsv_fronter_leds(void);
void set_hsv_fronter_value(uint8_t hue, uint8_t sat, uint8_t val);
// void set_hsv_fronter_leds(uint8_t hue, uint8_t sat, uint8_t val);
void set_hsv_leds(uint8_t hue, uint8_t sat, uint8_t val, bool indicator_leds);

//function to handle all the tap dances
int cur_dance(qk_tap_dance_state_t *state);

//functions for each tap dance
void tk_finished(qk_tap_dance_state_t *state, void *user_data);
void tk_reset(qk_tap_dance_state_t *state, void *user_data);


// https://www.reddit.com/r/olkb/comments/5s8q76/help_pro_micro_pinout_for_qmk/
// https://cdn.sparkfun.com/assets/9/c/3/c/4/523a1765757b7f5c6e8b4567.png
// #define INDICATOR_LED   B5

#define TX_LED   D5
#define RX_LED   B0

// Status LED Brightness
enum status_led_brightness_enum {
    _BASE_BRIGHTNESS = 1,
    _FUNCTION_LAYER_BRIGHTNESS = 8,
    _MEDIA_LAYER_BRIGHTNESS = 16,
};


// Layer Definitions
enum layer_number {
    _BASE = 0,
    _FUNCTION,
    _MEDIA,
    _RGB_LAYER
};


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    /* Base Layer  (Fronter colored)
    * ,--------------------.
    * |TapDan| Home | PgUp |
    * |------+------+------|
    * | Del  | End  | PgDn |
    * \--------------------/
    *        '------'
    *        |  Up  |
    * '------+------+------'
    * | Left | Down | Right|
    * '--------------------'
    */

    [_BASE] = LAYOUT(/* Base */
             TD(TAPPY_KEY), KC_HOME,  KC_PGUP,
                 KC_DEL,    KC_END,   KC_PGDN,
                 
                            KC_UP,
                 KC_LEFT,   KC_DOWN,  KC_RIGHT),


    /* function layer - Accessed with a Single Hold - Pink
    * ,--------------------.
    * |______| Mute | VolUp|
    * |------+------+------|
    * | Enter| Mute | VolDn|
    * \--------------------/
    *        '------'
    *        |______|
    * '------+------+------'
    * |______|______|______|
    * '--------------------'
    */
    [_FUNCTION] = LAYOUT(/* function layer */
                 KC_TRNS,   KC_VOLU,    XXXXXXX,
                 XXXXXXX,   KC_VOLD,    KC_MUTE,
                 
                            KC_TRNS,
                 KC_TRNS,   KC_TRNS,    KC_TRNS),


    /* media function layer - Accessed with a Single Tap  - Yellow
    * ,--------------------.
    * |______| Reset| VolUp|
    * |------+------+------|
    * | Mute |______| VolDn|
    * \--------------------/
    *        '------'
    *        | Space|
    * '------+------+------'
    * |  <<  | Play |  >>  |
    * '--------------------'
    */
    [_MEDIA] = LAYOUT(/* media function layer, toggled on a single tap */
                 KC_TRNS,   KC_VOLU,    XXXXXXX, 
                 XXXXXXX,   KC_VOLD,    KC_MUTE,
                 
                            KC_SPC ,
                 KC_MRWD,   KC_MPLY,    KC_MFFD),


    /* RGB layer, toggled on a double tap  - White
    * ,--------------------.
    * |______| Hue+ |Brght+|
    * |------+------+------|
    * | Togg | Hue- |Brght-|
    * \--------------------/
    *        '------'
    *        | SAT+ |
    * '------+------+------'
    * | Mode | SAT- |______|
    * '--------------------'
    */
    [_RGB_LAYER] = LAYOUT(/* RGB layer */
                 KC_TRNS,   RGB_HUI,    RGB_VAI,
                 RGB_TOG,   RGB_HUD,    RGB_VAD,
                 
                            RGB_SAI,
                 RGB_MOD,   RGB_SAD,    KC_TRNS),

};


void matrix_init_user(void) {
    //init the Pro Micro on-board LEDs
    setPinOutput(TX_LED);
    setPinOutput(RX_LED);
    //set to off
    writePinHigh(TX_LED);
    writePinHigh(RX_LED);
}

//turn on status led 
void keyboard_post_init_user(void) {

    debug_enable=true;
    // debug_matrix=true;

    // Call the post init code.
    #ifdef BACKLIGHT_ENABLE
        backlight_enable();
        backlight_level(_BASE_BRIGHTNESS);
        // breathing_enable();
        breathing_disable();
    #endif

    #ifdef RGBLIGHT_ENABLE
        // rgblight_setrgb_range(0.5, 0.5, 0.5, 0, 5);
        rgblight_enable_noeeprom(); // Enables RGB, without saving settings
        // rgblight_sethsv_noeeprom(DEFAULT_ARGB_COLOR);
        rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);

        // HSV tmp_hsv = {HSV_PURPLE};
        // fronter_hsv_value = tmp_hsv; 
        set_hsv_fronter_value(MEM_SWITCH_OUT_COLOR);
        set_hsv_leds(DEFAULT_ARGB_COLOR, true);
        
    #endif
}


// - RGB LED Funcs - 

void set_hsv_fronter_value(uint8_t hue, uint8_t sat, uint8_t val){
        HSV tmp_hsv = {hue, sat, val};
        fronter_hsv_value = tmp_hsv; 
} 

// void set_hsv_fronter_leds(void){

//     rgblight_sethsv_range(fronter_hsv_value.h, fronter_hsv_value.s, fronter_hsv_value.v, fronter_led_range.start, fronter_led_range.end+1);  // Start is inclusive, End is Exclusive

// }


void set_hsv_leds(uint8_t hue, uint8_t sat, uint8_t val, bool indicator_leds){

    // if (!indicator_leds)
    // {
    //     HSV tmp_hsv = {hue, sat, val};
    //     fronter_hsv_value = tmp_hsv; 
    // }
    

    for (uint8_t i = 0; i < RGBLED_NUM; i++) {
        if (i < fronter_led_range.start || i > fronter_led_range.end)
        {
            if(indicator_leds){
                sethsv(hue, sat, val, &led[i]);
            }
        }else{
            sethsv(fronter_hsv_value.h, fronter_hsv_value.s, fronter_hsv_value.v, &led[i]);
        }
    }
    rgblight_set();
}


// -- HID Code --

// void send_hid_debug(uint8_t *data){
//     raw_hid_send_command(CMD_PC_RAW_DEBUG_MSG, *data, sizeof(data))

// }

#ifdef RGBLIGHT_ENABLE
void set_rgblight_current_fronter(void){
    switch (current_fronter) {
        case MEM_SWITCHED_OUT:
            // rgblight_sethsv_noeeprom(HSV_WHITE);
            // set_hsv_fronter_value(HSV_WHITE);
            set_hsv_fronter_value(MEM_SWITCH_OUT_COLOR);
            break;
        case MEM_FLUTTERSHY:
            set_hsv_fronter_value(HSV_YELLOW);
            break;
        case MEM_HIBIKI:
            set_hsv_fronter_value(HSV_PURPLE);
            break;
        case MEM_LUNA:
            set_hsv_fronter_value(HSV_BLUE);
            break;
        case 255:
            set_hsv_fronter_value(HSV_RED);
            break;
    }
    // set_hsv_fronter_leds();
    set_hsv_leds(0, 0, 0, false);
}

// void set_rgblight_current_fronter(void){
    
//     if(current_fronter >= 4){
//         rgblight_sethsv_noeeprom(fav_colors[current_fronter].hue, fav_colors[current_fronter].sat, 255);
//     }else{
//         rgblight_sethsv_noeeprom(HSV_RED);
//     }
// }

// Sets all LEDs to the single HSV value sent from the PC
void set_rgblight_uniform_from_pc_cmd(uint8_t *hsv_data){
    /* 
    * Data Format:
    * Byte 0: Hue
    * Byte 1: Saturation
    * Byte 2: Value
    * */
    rgblight_sethsv_noeeprom(hsv_data[0], hsv_data[1], hsv_data[2]);
}

// https://github.com/qmk/qmk_firmware/blob/master/quantum/rgblight.h
// Sets each LED to the corresponding HSV value sent from the PC
void set_rgblight_from_pc_cmd(uint8_t *led_data, uint8_t length){
    /* 
    * Data Format:
    * Byte n+0: LED Number
    * Byte n+1: Hue
    * Byte n+2: Saturation
    * Byte n+3: Value
    * */

    // raw_hid_send_command(CMD_PC_RAW_DEBUG_MSG, led_data, length);

    // if (!rgblight_is_enabled()) {
    //     return;
    // }

   for (uint8_t i = 0; i < length; i+=4)
   {
       sethsv(led_data[i+1], led_data[i+2], led_data[i+3], (LED_TYPE *) &led[led_data[i]]);  // Hue, Sat, Val, LED Num
   }
   rgblight_set();

    // uint8_t dbug_msg[32] = {0};
    // uint8_t msg_cnt = 0;

    // dbug_msg[msg_cnt++] = length;
    // dbug_msg[msg_cnt++] = 42;

    // // uint8_t tmp_counter = 0;
    // for (uint8_t i = 0; i < length; i+=4)
    // {
    //     // tmp_counter++;
    //     // sethsv(0, 255, 255, (LED_TYPE *) &led[i+0]);  // RED, LED Num
    //     // uint8_t tmp[2] = {i+0, *led[i+0]};
    //     // uint8_t tmp[1] = {i+0};
    //     dbug_msg[msg_cnt++] = i;
    //     dbug_msg[msg_cnt++] = 42;

    //     // raw_hid_send_command(CMD_PC_RAW_DEBUG_MSG, tmp, sizeof(tmp));

    // }
}

#endif

// void raw_hid_send_response(void) {
//   // Send the current info screen index to the connected node script so that it can pass back the new data
//   uint8_t send_data[32] = {0};
//   uint8_t send_index = 0;
//   send_data[send_index++] = 2;  // Indicating response back

//   if(switch_fronter < 255){
//     send_data[send_index++] = 1; // command byte describing what next bytes do. 0: Nothing, 1: Switch
//     send_data[send_index++] = switch_fronter; // since the command byte was 1 (switch) this is the member who is switching.
//     switch_fronter = 255;  // Reset varible back to idle state
//   }
  
//   send_data[send_index++] = 251; // indicate end of response back.
//   raw_hid_send(send_data, sizeof(send_data));
// }


void raw_hid_send_command(uint8_t command_id, uint8_t *data, uint8_t length) {
/*
* Sent HID Data Packet Format:
* Byte 0: The Command Type
* Byte 1: Length of Command Data
* Byte 2-31: Command Data 
*
*/
    uint8_t send_data[32] = {0};  // data packet must be 32 bytes on 8bit AVR platform
    send_data[0] = command_id;
    send_data[1] = length;
    memcpy(&send_data[2], data, length);
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
    #ifdef CONSOLE_ENABLE
    // uprintf("Data Recived: len: %u, Data: [%u, %u, %u, %u, %u, %u, %u, %u, ..., %u, %u]\n", length, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], data[length-2], data[length-1]);
    #endif
    hid_connected = true; // PC connected
    // hid_disconection_timer = timer_read();  // Reset the timeout timer

    if (length >= 3) {
        uint8_t *command_id   = &(data[0]);
        uint8_t *data_length  = &(data[1]);
        uint8_t *command_data = &(data[2]);

        switch(*command_id){
            case CMD_DO_NOTHING:
                break;

            case CMD_KB_SET_CURRENT_FRONTER:
                if(current_fronter != command_data[0]){
                    current_fronter = command_data[0];
                    #ifdef RGBLIGHT_ENABLE
                    set_rgblight_current_fronter();
                    #endif
                }
                break;

            case CMD_KB_SET_ALL_RGB_LEDS:
                if (*data_length == 3){  // Data Len must be 3 for this function.
                    set_rgblight_uniform_from_pc_cmd(command_data);
                }else{
                    ; // TODO: Handle This error case
                }
                break;

            case CMD_KB_SET_RGB_LEDS:
                if(*data_length % 4 == 0){  // Data Len must be a multiple of 4 for this function.
                    set_rgblight_from_pc_cmd(command_data, *data_length);
                }else{
                    ; // TODO: Handle This error case
                }
                break;

            default:
                // We either recived a not yet supported HID call or a malformed packet. Do nothing.
                break;
        }
    }
}
// ----------------


//determine the current tap dance state
int cur_dance (qk_tap_dance_state_t *state){
    if(state->count == 1){
        //if a tap was registered
        if(!state->pressed){
            //if not still pressed, then was a single tap
            return SINGLE_TAP;
        } else {
            //if still pressed/held down, then it's a single hold
            return SINGLE_HOLD;
        }
    } else if(state->count == 2){
        //if tapped twice, set to double tap
        return DOUBLE_TAP;

    } else if(state->count == 3){
        //if tapped thrice, set to triple tap
        if (state->interrupted || !state->pressed) return TRIPLE_TAP;
        else return TRIPLE_HOLD;
    } else {
        return 8;
    }
}

//initialize the tap structure for the tap key
static tap tk_tap_state = {
    .is_press_action = true,
    .state = 0
};

//functions that control what our tap dance key does
void tk_finished(qk_tap_dance_state_t *state, void *user_data){
    tk_tap_state.state = cur_dance(state);
    switch(tk_tap_state.state){
        case SINGLE_TAP:
            //send desired key when tapped:
            //setting to the media layer
            if(layer_state_is(_MEDIA)){
                //if already active, toggle it to off
                layer_off(_MEDIA);
                xprintf("Turning Off Media Layer\n");
                //turn off the indicator LED
                //set LED HI to turn it off
                #ifdef INDICATOR_LED
                    writePinHigh(INDICATOR_LED);
                #endif

            } else {
                //turn on the media layer
                layer_move(_MEDIA);
                xprintf("Turning On Media Layer\n");
                //turn on the indicator LED
                //set LED pin to LOW to turn it on
                #ifdef INDICATOR_LED
                    writePinLow(INDICATOR_LED);
                #endif
            }
            break;
        case SINGLE_HOLD:
            //set to desired layer when held:
            //setting to the function layer
            layer_on(_FUNCTION);
            xprintf("Turning On Function Layer\n");
            break;

        case DOUBLE_TAP:
            //set to desired layer when double tapped:
            //setting to the RGB layer
            if(layer_state_is(_RGB_LAYER)){
                //if already active, toggle it to off
                layer_off(_RGB_LAYER);
                xprintf("Turning Off RGB Layer\n");
                //turn off the indicator LED
                //set LED HI to turn it off
                // writePinHigh(INDICATOR_LED);
            } else {
                //turn on the RGB layer
                layer_move(_RGB_LAYER);
                xprintf("Turning On RGB Layer\n");
                //turn on the indicator LED
                //set LED pin to LOW to turn it on
                // writePinLow(INDICATOR_LED);
            }
            break;

        case TRIPLE_TAP:
            xprintf("Reseting Layers\n");
            //reset all layers
            layer_clear();

            //set all LEDs off
            // writePinHigh(TX_LED);
            // writePinHigh(RX_LED);
            // writePinHigh(INDICATOR_LED);
            break;

        case TRIPLE_HOLD:
            rgblight_sethsv_noeeprom(HSV_RED);  // Set all to red to indicate RESET!
            wait_ms(100);
            reset_keyboard(); 
            break;
    }
}

void tk_reset(qk_tap_dance_state_t *state, void *user_data){
    //if held and released, leave the layer
    if(tk_tap_state.state == SINGLE_HOLD){
        layer_off(_FUNCTION);
    }
    //reset the state
    tk_tap_state.state = 0; 
}

//associate the tap dance key with its functionality
qk_tap_dance_action_t tap_dance_actions[] = {
    [TAPPY_KEY] = ACTION_TAP_DANCE_FN_ADVANCED_TIME(NULL, tk_finished, tk_reset, 275)
};


layer_state_t layer_state_set_user(layer_state_t state) {

    #ifdef BACKLIGHT_ENABLE
    // Set status LED brightness based on current layer
    switch (get_highest_layer(state)) {
        case _BASE:
            backlight_level(_BASE_BRIGHTNESS);
            // set_rgblight_current_fronter(); // Purple
            // set_hsv_fronter_leds();
            set_hsv_leds(0, 0, 0, false);
            set_hsv_leds(DEFAULT_ARGB_COLOR, true);
            writePinHigh(TX_LED);
            writePinHigh(RX_LED);
            print("RGB: Purple\n");
            break;
        case _FUNCTION:
            backlight_level(_FUNCTION_LAYER_BRIGHTNESS);
            // rgblight_sethsv_noeeprom(HSV_MAGENTA);
            set_hsv_leds(HSV_MAGENTA, true);
            writePinHigh(TX_LED);
            writePinHigh(RX_LED);
            print("RGB: Blue\n");
            break;
        case _MEDIA:
            backlight_level(_MEDIA_LAYER_BRIGHTNESS);
            // rgblight_sethsv_noeeprom(HSV_YELLOW);
            set_hsv_leds(HSV_YELLOW, true);
            writePinHigh(TX_LED);
            writePinHigh(RX_LED);
            print("RGB: Yellow\n");
            break;

        case _RGB_LAYER:
            backlight_level(_BASE_BRIGHTNESS);
            // rgblight_sethsv_noeeprom(HSV_WHITE);
            set_hsv_leds(HSV_WHITE, true);
            writePinLow(TX_LED);
            writePinLow(RX_LED);
            print("RGB: Pink\n");
            break;
  
        default:
            backlight_level(_BASE_BRIGHTNESS);
            // rgblight_sethsv_noeeprom(DEFAULT_ARGB_COLOR);
            set_hsv_leds(DEFAULT_ARGB_COLOR, true);
            print("RGB: Purple (default)\n");
            break;
    }
    #endif

    return state;
}