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

// Default Backlight Color
#define DEFAULT_ARGB_COLOR HSV_PURPLE

//create the tap type
typedef struct {
    bool is_press_action;
    int state;
} tap;

//tap dance states
enum {
    SINGLE_TAP = 1,
    SINGLE_HOLD = 2,
    DOUBLE_TAP = 3,
    TRIPLE_TAP = 4
};

//tap dance keys
enum {
    TAPPY_KEY = 0
};


//function to handle all the tap dances
int cur_dance(qk_tap_dance_state_t *state);

//functions for each tap dance
void tk_finished(qk_tap_dance_state_t *state, void *user_data);
void tk_reset(qk_tap_dance_state_t *state, void *user_data);


// https://www.reddit.com/r/olkb/comments/5s8q76/help_pro_micro_pinout_for_qmk/
// https://cdn.sparkfun.com/assets/9/c/3/c/4/523a1765757b7f5c6e8b4567.png
// #define INDICATOR_LED   B5

#define TX_LED          D5
#define RX_LED          B0

// Status LED Brightness
enum status_led_brightness_enum {
    _BASE_BRIGHTNESS = 1,
    _FN_LAYER_BRIGHTNESS = 8,
    _ML_LAYER_BRIGHTNESS = 16,
};


// Layer Definitions
enum layer_number {
  _BASE = 0,
  _FN0,
  _ML1,
  _RGB_LAYER
};


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    /* Base Layer
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
                 TD(TAPPY_KEY),KC_HOME, KC_PGUP,
                 KC_DEL,    KC_END,     KC_PGDN,
                 
                            KC_UP,
                 KC_LEFT,   KC_DOWN,    KC_RIGHT),


    /* function layer 
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
    [_FN0] = LAYOUT(/* function layer */
                 KC_TRNS,   KC_MUTE,    KC_VOLU,
                 KC_ENTER,  KC_MUTE,    KC_VOLD,
                 
                            KC_TRNS,
                 KC_TRNS,   KC_TRNS,    KC_TRNS),


    /* media function layer 
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
    [_ML1] = LAYOUT(/* media function layer, toggled on a single tap */
                 KC_TRNS,   RESET  ,    KC_VOLU, 
                 KC_MUTE,   KC_TRNS,    KC_VOLD,
                 
                            KC_SPC ,
                 KC_MRWD,   KC_MPLY,    KC_MFFD),

    /* RGB layer 
    * ,--------------------.
    * |______|toggle| Mode |
    * |------+------+------|
    * | Hue  | Sat  | Vibr |
    * \--------------------/
    *        '------'
    *        |______|
    * '------+------+------'
    * |______|______|______|
    * '--------------------'
    */
    [_RGB_LAYER] = LAYOUT(/* RGB layer */
                 KC_TRNS,   RGB_TOG,    RGB_MOD,
                 RGB_HUI,   RGB_SAI,    RGB_VAI,
                 
                            KC_TRNS,
                 KC_TRNS,   KC_TRNS,    KC_TRNS),

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
        rgblight_sethsv_noeeprom(DEFAULT_ARGB_COLOR);
        rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
    #endif
}


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
        return TRIPLE_TAP;
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
            if(layer_state_is(_ML1)){
                //if already active, toggle it to off
                layer_off(_ML1);
                xprintf("Turning Off Media Layer\n");
                //turn off the indicator LED
                //set LED HI to turn it off
                #ifdef INDICATOR_LED
                    writePinHigh(INDICATOR_LED);
                #endif

            } else {
                //turn on the media layer
                layer_move(_ML1);
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
            layer_on(_FN0);
            xprintf("Turning On FN Layer\n");
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
    }
}

void tk_reset(qk_tap_dance_state_t *state, void *user_data){
    //if held and released, leave the layer
    if(tk_tap_state.state == SINGLE_HOLD){
        layer_off(_FN0);
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
            rgblight_sethsv_noeeprom(DEFAULT_ARGB_COLOR);  // Purple
            writePinHigh(TX_LED);
            writePinHigh(RX_LED);
            print("RGB: Purple\n");
            break;
        case _FN0:
            backlight_level(_FN_LAYER_BRIGHTNESS);
            rgblight_sethsv_noeeprom(HSV_BLUE);
            writePinHigh(TX_LED);
            writePinHigh(RX_LED);
            print("RGB: Blue\n");
            break;
        case _ML1:
            backlight_level(_ML_LAYER_BRIGHTNESS);
            rgblight_sethsv_noeeprom(HSV_YELLOW);
            writePinHigh(TX_LED);
            writePinHigh(RX_LED);
            print("RGB: Yellow\n");
            break;

        case _RGB_LAYER:
            backlight_level(_BASE_BRIGHTNESS);
            rgblight_sethsv_noeeprom(HSV_PINK);
            writePinLow(TX_LED);
            writePinLow(RX_LED);
            print("RGB: Pink\n");
            break;
  
        default:
            backlight_level(_BASE_BRIGHTNESS);
            rgblight_sethsv_noeeprom(DEFAULT_ARGB_COLOR);
            print("RGB: Purple (default)\n");
            break;
    }
    #endif

    return state;
}