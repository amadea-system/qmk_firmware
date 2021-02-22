/*
This is the c configuration file for the keymap

Copyright 2020 Amadea System

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

// Backlight Config
#define BACKLIGHT_LEVELS 16      //  The number of brightness levels (maximum 31 excluding off)
#define BACKLIGHT_LIMIT_VAL 100 //128
#define BACKLIGHT_BREATHING
#define BREATHING_PERIOD 6
#define BACKLIGHT_PIN B5
#define BACKLIGHT_ON_STATE 0

// ARGB Underlight Config
#undef RGBLED_NUM
#define RGBLED_NUM 10
#define RGB_DI_PIN F4
#define RGBLIGHT_LIMIT_VAL 100

#undef RGBLIGHT_ANIMATIONS


// -- OLED config --
#define OLED_DISPLAY_128X64
#define OLED_UPDATE_INTERVAL 50   // Sets the update rate of the OLED display in ms. Greatly improves the matrix scan rate.
#define OLED_TIMEOUT 0            // We are implementing our own timeout since the way we are drawing to the screan is preventing the builting one from working.
#define OLED_USER_TIMEOUT 120000
#define OLED_DIMOUT 60000
// #define OLED_FONT_H "font.h"


/* -- Encoder Configuration -- */

#define ENCODERS_PAD_A { D0 }
#define ENCODERS_PAD_B { D1 }
#define ENC_SW_PIN_1 C6

// We need to redefine the matrix to inser the encoder as a DirectPin.
/* key matrix size */
#undef MATRIX_ROWS
#undef MATRIX_COLS

#define MATRIX_ROWS 4
#define MATRIX_COLS 4  // 3
#define MATRIX_MASKED 

/* Keyboard Matrix Assignments */
#undef MATRIX_ROW_PINS
#undef MATRIX_COL_PINS
#define MATRIX_ROW_PINS  { B6, B2, B3, B4 }
#define MATRIX_COL_PINS  { D4, E6, B1, ENC_SW_PIN_1 }  // Encoders: C6  // { D4, E6, B1 }


#define LAYOUT_W_ENC( \
    k0, k1, k2, k10,\
    k3, k4, k5,\
               \
        k6,    \
    k7, k8, k9 \
) \
{ \
    {k0,    k1, k2,    k10}, \
    {k3,    k4, k5,    KC_NO}, \
    {KC_NO, k6, KC_NO, KC_NO}, \
    {k7,    k8, k9,    KC_NO}, \
}


