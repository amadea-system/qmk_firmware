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


// #define NUMBER_OF_ENCODERS (sizeof(encoder_sw_pin) / sizeof(pin_t))
// static pin_t encoder_sw_pin[] = ENCODERS_BUTTON_DIRECT_PIN;
// static uint8_t encoder_sw_state[NUMBER_OF_ENCODERS] = {0};

const matrix_row_t matrix_mask[MATRIX_ROWS] =
{// ----4321
  0b00001111,  //0b11111111: C6, 
  0b00000111,
  0b00000010,
  0b00000111,
};

// -------- Function Defs ---------//
// void encoder_init_sw(void);
// void encoder_read_sw(void);
// void encoder_sw_update_user(uint8_t index, bool pressed);

// --------

#define INDICATOR_LED   B5
#define TX_LED   D5
#define RX_LED   B0

#define _TKL    0
#define _FN0    1
#define _ML1    2

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [_TKL] = LAYOUT_W_ENC(/* Base */
                    KC_A,    KC_B,    KC_D,  KC_1,
                    KC_E,    KC_F,    KC_G,  KC_2,
                 
                             KC_H,           KC_3,
                    KC_I,    KC_J,    KC_K,  KC_4),
                 
    // [_FN0] = LAYOUT(/* function layer */
    //              KC_TRNS,   KC_PAUS,    KC_VOLU,
    //              KC_ENTER,  KC_SLCK,    KC_VOLD,
                 
    //                         KC_TRNS,
    //              KC_TRNS,   KC_TRNS,    KC_TRNS),
    // [_ML1] = LAYOUT(/* media function layer, toggled on a single tap */
    //              KC_TRNS,   KC_TRNS,    KC_VOLU, 
    //              KC_MUTE,   KC_TRNS,    KC_VOLD,
                 
    //                         KC_SPC,
    //              KC_MRWD,   KC_MPLY,    KC_MFFD),
};


/* - Keyboard Initilization - */
void matrix_init_user(void) {
    //init the Pro Micro on-board LEDs
    setPinOutput(TX_LED);
    setPinOutput(RX_LED);
    //set to off
    writePinHigh(TX_LED);
    writePinHigh(RX_LED);
    // encoder_init_sw();
}

// Direct Pin Encoder Switch Support
// void encoder_init_sw(void){
//     for (int i = 0; i < NUMBER_OF_ENCODERS; i++) {
//         setPinInputHigh(encoder_sw_pin[i]);
//         // encoder_state[i] = (readPin(encoders_pad_a[i]) << 0) | (readPin(encoders_pad_b[i]) << 1);
//         encoder_sw_state[i] = readPin(encoder_sw_pin[i]);
//     }
// }

// void encoder_read_sw(void) {
//     for (uint8_t i = 0; i < NUMBER_OF_ENCODERS; i++) {
//         uint8_t updated_state = readPin(encoder_sw_pin[i]);
        
//         if(encoder_sw_state[i] != updated_state){
//             encoder_sw_update_user(i, !updated_state);
//         }
//         encoder_sw_state[i] = updated_state;
//     }
// }

//- Direct Pin Encoder Switch Support


void matrix_scan_user(void) {

    // encoder_read_sw();
}


void turn_leds_on(void){
    //set LED pin to LOW to turn it on
    writePinLow(INDICATOR_LED);
    writePinLow(TX_LED);
    writePinLow(RX_LED);

}

void turn_leds_off(void){
    //set LED HI to turn it off
    writePinHigh(INDICATOR_LED);
    writePinHigh(TX_LED);
    writePinHigh(RX_LED);
}

// void encoder_sw_update_user(uint8_t index, bool pressed){
//     switch (index)
//     {
//     case 0:
//         if(pressed){
//             register_code(KC_Y);
//         }else{
//             unregister_code(KC_Y);
//         }
//         break;
//     case 1:
//         if(pressed){
//             register_code(KC_Q);
//         }else{
//             unregister_code(KC_Q);
//         }
//         break;
    
//     default:
//         break;
//     }   
// }

void encoder_update_user(uint8_t index, bool clockwise) {

    if (clockwise) {
        turn_leds_off();
        tap_code(KC_Z);  // Right
        
    }else{
        turn_leds_on();
        tap_code(KC_X);  // Left
    }

}