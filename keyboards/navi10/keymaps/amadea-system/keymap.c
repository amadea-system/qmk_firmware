/* Copyright 2019 Ethan Durrant (emdarcher)
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
 * 
 * 
 * 
 *  --- Notes ---
 * 
 * Pro Micro Min Maps
 * https://www.reddit.com/r/olkb/comments/5s8q76/help_pro_micro_pinout_for_qmk/
 * https://cdn.sparkfun.com/assets/9/c/3/c/4/523a1765757b7f5c6e8b4567.png
 * 
 */


#include QMK_KEYBOARD_H
#include "print.h"
#include "raw_hid.h"

#include "string.h" // For memcpy
#include "stdio.h"

#include "navi10_keymap_enums.h"


/* ------------ # Defines ------------ */

#define DEFAULT_ARGB_COLOR HSV_SPRINGGREEN  // Default Backlight Color, Used for the Base Layer
#define MEM_SWITCH_OUT_COLOR HSV_BLUE       // The Backlight color to indicate "No one in front". Defined here becuase it is used in several places.

// - Pin Defs -
#define TX_LED   D5
#define RX_LED   B0
// #define INDICATOR_LED   B5

// - HID -
#define HID_DISCONECTION_TIMEOUT 5000  // milliseconds 

// - OLED -
// #define OLED_BRIGHTNESS 255


/* ----- Local Enums & Type Defs ----- */

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
}secondary_led_range_t;


/* --------- Local Variables --------- */

// - RGB LED Variables - 
#define SECONDARY_LEDS_INDICATION
// #define SECONDARY_LEDS_FRONTER
secondary_led_range_t secondary_led_range = {4, 5};
HSV fronter_hsv_value;


// - HID Variables - 
bool hid_connected = false; // Flag indicating if we have a PC connection yet
uint8_t current_fronter = MEM_SWITCHED_OUT;
static uint16_t hid_disconection_timer = 0;
static uint16_t activity_ping_timer = 0;


// - OLED Variables - 
static uint32_t oled_user_timeout = 0;
// static uint16_t oled_countdown_timer = 0;


/* ----------- Function Defs --------- */

// - HID Funcs - 
void send_hid_debug(uint8_t *data);
void raw_hid_send_command(uint8_t command_id, uint8_t *data, uint8_t length);
bool check_hid_timeout(void);

// - RGB LED Funcs - 
// void set_hsv_fronter_leds(void);
void set_hsv_fronter_value(uint8_t hue, uint8_t sat, uint8_t val);
// void set_hsv_fronter_leds(uint8_t hue, uint8_t sat, uint8_t val);
void set_hsv_leds(uint8_t hue, uint8_t sat, uint8_t val, bool indicator_leds);

// - Tap Dance Funcs - 
//function to handle all the tap dances
int cur_dance(qk_tap_dance_state_t *state);

//functions for each tap dance
void tk_finished(qk_tap_dance_state_t *state, void *user_data);
void tk_reset(qk_tap_dance_state_t *state, void *user_data);


/* -------- KeyMap Definition -------- */

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
    * |______| VolUp|      |
    * |------+------+------|
    * |      | VolDn| Mute |
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
    * |______| VolUp|      |
    * |------+------+------|
    * |      | VolDn| Mute |
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
    * | Mode | SAT- |      |
    * '--------------------'
    */
    [_RGB_LAYER] = LAYOUT(/* RGB layer */
                 KC_TRNS,   RGB_HUI,    RGB_VAI,
                 RGB_TOG,   RGB_HUD,    RGB_VAD,
                 
                            RGB_SAI,
                 RGB_MOD,   RGB_SAD,    XXXXXXX),

};


/* ------------ Functions ------------ */

/* - Keyboard Initilization - */

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

    debug_enable=false;
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
// -----------


/* - RGB LED Functions - */

#ifdef RGBLIGHT_ENABLE
void set_hsv_fronter_value(uint8_t hue, uint8_t sat, uint8_t val){
        HSV tmp_hsv = {hue, sat, val};
        fronter_hsv_value = tmp_hsv; 
} 


// void set_hsv_fronter_leds(void){

//     rgblight_sethsv_range(fronter_hsv_value.h, fronter_hsv_value.s, fronter_hsv_value.v, fronter_led_range.start, fronter_led_range.end+1);  // Start is inclusive, End is Exclusive

// }


/*  void set_hsv_leds(uint8_t hue, uint8_t sat, uint8_t val, bool indicator_leds)
 * 
 *  indicator_leds: If True, Set the LEDs used for indication (Layer, Caps lock, Etc...) to the provided HSV values
 *                  If False, Only update the fronter LEDs.
 * 
 */
void set_hsv_leds(uint8_t hue, uint8_t sat, uint8_t val, bool indicator_leds){

    // if (!indicator_leds)
    // {
    //     HSV tmp_hsv = {hue, sat, val};
    //     fronter_hsv_value = tmp_hsv; 
    // }

 
    #ifdef SECONDARY_LEDS_FRONTER
    
    /* Set Config LED Range to Fronter Values, Non-Config range to passed HSV Values */
    for (uint8_t i = 0; i < RGBLED_NUM; i++) {
        if (i < secondary_led_range.start || i > secondary_led_range.end)
        {
            if(indicator_leds){
                sethsv(hue, sat, val, &led[i]);
            }
        }else{
            sethsv(fronter_hsv_value.h, fronter_hsv_value.s, fronter_hsv_value.v, &led[i]);
        }
    }

    #else // SECONDARY_LEDS_INDICATION
    /* Set Config LED Range to passed HSV Values, Non-Config range to Fronter Values */
    for (uint8_t i = 0; i < RGBLED_NUM; i++) {
        if (i < secondary_led_range.start || i > secondary_led_range.end)
        {
            sethsv(fronter_hsv_value.h, fronter_hsv_value.s, fronter_hsv_value.v, &led[i]);
        }else{
            if(indicator_leds){
                sethsv(hue, sat, val, &led[i]);
            }
        }
    }
    #endif
    
    rgblight_set();
}

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

#endif  // -RGBLIGHT_ENABLE

// -----------


/* - HID Functions - */

// These RGB LED Functions are in the HID section since they are HID Based functions.
#ifdef RGBLIGHT_ENABLE
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
#endif  // -RGBLIGHT_ENABLE

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

// All hid Timer code clocks in at approx 47 bytes
bool check_hid_timeout(void){

    if (!hid_connected || timer_elapsed(hid_disconection_timer) >= HID_DISCONECTION_TIMEOUT) {
        hid_connected = false;
        return false;
    }
    return true;
}


void send_hid_debug(uint8_t *data){
    raw_hid_send_command(CMD_PC_RAW_DEBUG_MSG, data, sizeof(data));
}


void hid_send_activity_ping(void){

    if (!hid_connected || timer_elapsed(activity_ping_timer) < 5000) {
        // Only send a ping once every 5 seconds and only if we are connected.
        return;
    }

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
    hid_disconection_timer = timer_read();  // Reset the timeout timer

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

            case CMD_KB_ACTIVITY_PING:
                #if OLED_USER_TIMEOUT > 0
                oled_user_timeout = timer_read32(); // feed the oled timer
                #endif
                break;


            default:
                // We either recived a not yet supported HID call or a malformed packet. Do nothing.
                break;
        }
    }
}

// ----------------

/* - Tap Dance Functions - */

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

// -----------


bool process_record_user(uint16_t keycode, keyrecord_t *record) {  
    #if OLED_USER_TIMEOUT > 0
    oled_user_timeout = timer_read32(); // + OLED_USER_TIMEOUT;
    #endif

    hid_send_activity_ping();

    // switch (keycode) {

    // }

    return true;

}

/* - Layer State Functions - */

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

// -----------


/* - OLED Functions - */

#ifdef OLED_DRIVER_ENABLE

oled_rotation_t oled_init_user(oled_rotation_t rotation) {

    oled_user_timeout = timer_read32();// + OLED_USER_TIMEOUT;
    return OLED_ROTATION_0; 
}



// void oled_decrease_brightness(void){
//     uint8_t current_brightness = oled_get_brightness();

//     int16_t new_brightness = current_brightness - 5;   
//     if(new_brightness <= 0){
//         new_brightness = 0;
//         oled_fade_direction = 0;
//     }

//     oled_set_brightness((uint8_t)new_brightness);
// }

// void oled_increase_brightness(void){
//     uint8_t current_brightness = oled_get_brightness();

//     uint16_t new_brightness = current_brightness + 5;
//     if(new_brightness >= 255){
//         new_brightness = 255;
//         oled_fade_direction = 0;
//     }
//     oled_set_brightness((uint8_t)new_brightness);    
// }

void oled_fade_set_brightness(uint8_t level, uint8_t rate){
    uint8_t current_brightness = oled_get_brightness();
    if(current_brightness > level){
        int16_t new_brightness = current_brightness - rate;   
        if(new_brightness <= level){
            new_brightness = level;
        }
        oled_set_brightness((uint8_t)new_brightness);
    }else if(current_brightness < level){
        uint16_t new_brightness = current_brightness + rate;
        if(new_brightness >= level){
            new_brightness = level;
        }
        oled_set_brightness((uint8_t)new_brightness);
    }
}


void oled_render_backlight_brightness(void){
    // Debug function

    uint8_t current_brightness = oled_get_brightness();
    char bright_buff[30];
    snprintf(bright_buff, sizeof(bright_buff), "OLED Br: %u", current_brightness);
    oled_write_ln(bright_buff, false);
}

void oled_render_timeout(void){
    // Debug function

    //  oled_timeout = timer_read32() + OLED_TIMEOUT;
    uint32_t time_remaining = timer_elapsed32(oled_user_timeout);
    char time_buff[30];
    snprintf(time_buff, sizeof(time_buff), "%lu, %lu,  %lu", time_remaining, OLED_USER_TIMEOUT, oled_user_timeout);
    oled_write_ln(time_buff, false);
}

void oled_render_hid_timeout(void){
    // Debug function
    
    //  oled_timeout = timer_read32() + OLED_TIMEOUT;
    uint16_t time_remaining = timer_elapsed(hid_disconection_timer);
    char time_buff[30];
    snprintf(time_buff, sizeof(time_buff), "%u,   %u", time_remaining, hid_disconection_timer);
    oled_write_ln(time_buff, false);
}

bool oled_inactivity_check(void){
    /*
     Checks if the oled should be dimmed or turned off.
     returns true if it should be off so we can avoid wtiting to the screen when off.
     */
    #if OLED_USER_TIMEOUT > 0

    if(timer_elapsed32(oled_user_timeout) > OLED_USER_TIMEOUT) {
        oled_off();
        return true;
    }else if(timer_elapsed32(oled_user_timeout) > OLED_DIMOUT){
        // oled_write_ln_P(PSTR("Dimout!"), false);
        oled_fade_set_brightness(1, 5);
    }else{
        // oled_write_ln_P(PSTR("Normal!!!"), false);
        oled_fade_set_brightness(OLED_BRIGHTNESS, 15);
    }
    #endif

    return false;
}

// static void render_kyria_logo(void) {
//     static const char PROGMEM kyria_logo[] = {
//         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,192,224,240,112,120, 56, 60, 28, 30, 14, 14, 14,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7, 14, 14, 14, 30, 28, 60, 56,120,112,240,224,192,128,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
//         0,  0,  0,  0,  0,  0,  0,192,224,240,124, 62, 31, 15,  7,  3,  1,128,192,224,240,120, 56, 60, 28, 30, 14, 14,  7,  7,135,231,127, 31,255,255, 31,127,231,135,  7,  7, 14, 14, 30, 28, 60, 56,120,240,224,192,128,  1,  3,  7, 15, 31, 62,124,240,224,192,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
//         0,  0,  0,  0,240,252,255, 31,  7,  1,  0,  0,192,240,252,254,255,247,243,177,176, 48, 48, 48, 48, 48, 48, 48,120,254,135,  1,  0,  0,255,255,  0,  0,  1,135,254,120, 48, 48, 48, 48, 48, 48, 48,176,177,243,247,255,254,252,240,192,  0,  0,  1,  7, 31,255,252,240,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
//         0,  0,  0,255,255,255,  0,  0,  0,  0,  0,254,255,255,  1,  1,  7, 30,120,225,129,131,131,134,134,140,140,152,152,177,183,254,248,224,255,255,224,248,254,183,177,152,152,140,140,134,134,131,131,129,225,120, 30,  7,  1,  1,255,255,254,  0,  0,  0,  0,  0,255,255,255,  0,  0,  0,  0,255,255,  0,  0,192,192, 48, 48,  0,  0,240,240,  0,  0,  0,  0,  0,  0,240,240,  0,  0,240,240,192,192, 48, 48, 48, 48,192,192,  0,  0, 48, 48,243,243,  0,  0,  0,  0,  0,  0, 48, 48, 48, 48, 48, 48,192,192,  0,  0,  0,  0,  0,
//         0,  0,  0,255,255,255,  0,  0,  0,  0,  0,127,255,255,128,128,224,120, 30,135,129,193,193, 97, 97, 49, 49, 25, 25,141,237,127, 31,  7,255,255,  7, 31,127,237,141, 25, 25, 49, 49, 97, 97,193,193,129,135, 30,120,224,128,128,255,255,127,  0,  0,  0,  0,  0,255,255,255,  0,  0,  0,  0, 63, 63,  3,  3, 12, 12, 48, 48,  0,  0,  0,  0, 51, 51, 51, 51, 51, 51, 15, 15,  0,  0, 63, 63,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 48, 48, 63, 63, 48, 48,  0,  0, 12, 12, 51, 51, 51, 51, 51, 51, 63, 63,  0,  0,  0,  0,  0,
//         0,  0,  0,  0, 15, 63,255,248,224,128,  0,  0,  3, 15, 63,127,255,239,207,141, 13, 12, 12, 12, 12, 12, 12, 12, 30,127,225,128,  0,  0,255,255,  0,  0,128,225,127, 30, 12, 12, 12, 12, 12, 12, 12, 13,141,207,239,255,127, 63, 15,  3,  0,  0,128,224,248,255, 63, 15,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
//         0,  0,  0,  0,  0,  0,  0,  3,  7, 15, 62,124,248,240,224,192,128,  1,  3,  7, 15, 30, 28, 60, 56,120,112,112,224,224,225,231,254,248,255,255,248,254,231,225,224,224,112,112,120, 56, 60, 28, 30, 15,  7,  3,  1,128,192,224,240,248,124, 62, 15,  7,  3,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
//         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  3,  7, 15, 14, 30, 28, 60, 56,120,112,112,112,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,112,112,112,120, 56, 60, 28, 30, 14, 15,  7,  3,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
//     };
//     oled_write_raw_P(kyria_logo, sizeof(kyria_logo));
// }


static void render_qmk_logo(bool render_at_bottom) {
    /* 3 lines tall */
    if(!is_oled_on()){
        return;
    }

    if(render_at_bottom){
        oled_set_cursor(0,5);  // Go to line 6128
    }
    static const char PROGMEM qmk_logo[] = {
      0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,0x90,0x91,0x92,0x93,0x94,
      0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xab,0xac,0xad,0xae,0xaf,0xb0,0xb1,0xb2,0xb3,0xb4,
      0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xcb,0xcc,0xcd,0xce,0xcf,0xd0,0xd1,0xd2,0xd3,0xd4,0};

    oled_write_ln_P(qmk_logo, false);

    if(render_at_bottom){
        oled_set_cursor(0,0);  // Go back to line 1
    }
}


void render_layer_state(void) {
    /* 12 char long  (9 char left on line)*/

    // oled_write_P(PSTR("Layer: "), false);
    // Layer name must be 5 characters long.
    if(get_highest_layer(layer_state) == _BASE){
        oled_write_P(PSTR("Layer: "), false);
    }else{
        // oled_set_cursor(16, 0);
        // oled_write_P(PSTR("                "), false);
        oled_write_P(PSTR("----------------"), false);
    }

    switch (get_highest_layer(layer_state)) {
        case _BASE:
            oled_write_ln_P(PSTR("TKL  "), false);
            break;
        case _FUNCTION:
            oled_write_P(PSTR(" Func"), false);
            break;
        case _MEDIA:
            oled_write_P(PSTR("Media"), false);
            break;
        case _RGB_LAYER:
            oled_write_P(PSTR("  RGB"), false);
            break;
        default:
            oled_write_P(PSTR("Undef"), false);
    }

    // oled_write_ln_P(PSTR(""), false);
}


void render_current_fronter(void){

  if (check_hid_timeout()){

    oled_write_P(PSTR("Amadea Sys    "), false);

    // Member name must be 7 characters long.
    switch (current_fronter) {
        case MEM_SWITCHED_OUT:
            oled_write_P(PSTR("  Sleep"), false);
            break;
        case MEM_FLUTTERSHY:
            oled_write_P(PSTR("Flutter"), false);
            break;
        case MEM_HIBIKI:
            oled_write_P(PSTR(" Hibiki"), false);
            break;
        case MEM_LUNA:
            oled_write_P(PSTR("   Luna"), false);
            // oled_write_P(PSTR("Flutter"), false);
            break;
        case 255:
            oled_write_P(PSTR("  Error"), false);
            break;
    }
  }else{
    oled_write_P(PSTR("    Amadea System    "), false);
  }
}

void render_default_screen(void){
    render_qmk_logo(true);  // Must be first!

    if(oled_inactivity_check()){
        return;
    }

    render_current_fronter();
    oled_write_ln_P(PSTR(""), false);

    render_layer_state();

    // make sure the text from the keyhints are cleared
    oled_write_ln_P(PSTR(""), false);
    oled_write_ln_P(PSTR(""), false);
}

void render_keyhint_horz_sep(void){
    oled_write_ln_P(PSTR("------+------+------"), false);
}

// void render_keyhint_key(const char *key_text, bool last_key_in_row){
//     oled_write_P(key_text, false);

//     if (last_key_in_row){
//         oled_advance_page(true);
//     }else{
//         oled_write_P(PSTR("|"), false);
//     }
// }

void render_keyhint_keyRow(const char *first_key, const char *second_key, const char *third_key){

    /* Doing a single direct oled_write would be more efficent (codespace) but this is nicer. */
    oled_write_P(first_key, false);
    oled_write_P(PSTR("|"), false);
    oled_write_P(second_key, false);
    oled_write_P(PSTR("|"), false);
    oled_write_ln_P(third_key, false);
}


#define BLANK_KEY PSTR("      ")
#define __BLANK__KEY__ BLANK_KEY
void render_keyhints_L_fn(void){

    render_layer_state();

    render_keyhint_keyRow(PSTR(" Tappy"), PSTR(" VolUp"), __BLANK__KEY__);
    render_keyhint_horz_sep();

    render_keyhint_keyRow(__BLANK__KEY__, PSTR(" VolDn"), PSTR(" Mute "));
    render_keyhint_horz_sep();

    render_keyhint_keyRow(__BLANK__KEY__, PSTR("  Up  "), __BLANK__KEY__);
    render_keyhint_horz_sep();

    render_keyhint_keyRow(PSTR(" Left "), PSTR(" Down "), PSTR(" Right"));


    // oled_write_ln_P(PSTR(" Tappy| VolUp|      "), false);
    // // oled_write_ln_P(PSTR("------+------+------"), false);
    // render_keyhint_horz_sep();
    // oled_write_ln_P(PSTR("      | VolDn| Mute "), false);
    // // oled_write_ln_P(PSTR("------+------+------"), false);
    // render_keyhint_horz_sep();
    // oled_write_ln_P(PSTR("      |  Up  |      "), false);
    // // oled_write_ln_P(PSTR("------+------+------"), false);
    // render_keyhint_horz_sep();
    // oled_write_ln_P(PSTR(" Left | Down | Right"), false);

   
}

void render_keyhints_L_media(void){

    render_layer_state();

    render_keyhint_keyRow(PSTR(" Tappy"), PSTR(" VolUp"), BLANK_KEY);
    render_keyhint_horz_sep();

    render_keyhint_keyRow(BLANK_KEY,      PSTR(" VolDn"), PSTR(" Mute "));
    render_keyhint_horz_sep();

    render_keyhint_keyRow(BLANK_KEY,      PSTR(" Space"), BLANK_KEY);
    render_keyhint_horz_sep();

    render_keyhint_keyRow(PSTR("  <<  "), PSTR(" Play "), PSTR("  >>  "));
}

void render_keyhints_L_RGB(void){

    render_layer_state();

    render_keyhint_keyRow(PSTR(" Tappy"), PSTR(" Hue+ "), PSTR("Brght+"));
    render_keyhint_horz_sep();

    render_keyhint_keyRow(PSTR(" RGB  "),  PSTR(" Hue- "), PSTR("Brght-"));
    render_keyhint_horz_sep();

    render_keyhint_keyRow(__BLANK__KEY__, PSTR(" Sat+ "), __BLANK__KEY__);
    render_keyhint_horz_sep();

    render_keyhint_keyRow(PSTR(" Mode "), PSTR(" Sat- "), __BLANK__KEY__);
}


void oled_task_user(void) {
    /* 21 character screen width. 7 lines high*/

    switch (get_highest_layer(layer_state)) {
        case _BASE:               
            render_default_screen();
            break;
        case _FUNCTION:
            render_keyhints_L_fn();
            break;
        case _MEDIA:
            render_keyhints_L_media();
            break;
        case _RGB_LAYER:
            render_keyhints_L_RGB();
            break;
        default:
            render_default_screen();
    }
    
}

#endif  // -OLED_DRIVER_ENABLE
