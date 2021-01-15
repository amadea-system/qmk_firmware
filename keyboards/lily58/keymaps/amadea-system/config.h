/*
This is the c configuration file for the keymap

Copyright 2012 Jun Wako <wakojun@gmail.com>
Copyright 2015 Jack Humbert

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

//#define USE_MATRIX_I2C

/* Select hand configuration */

#define MASTER_LEFT
// #define MASTER_RIGHT
// #define EE_HANDS

#define USE_SERIAL_PD2

#define TAPPING_FORCE_HOLD
#define TAPPING_TERM 100

// -- Underglow Config --
#undef RGBLED_NUM
#define RGBLED_NUM 10
#undef RGBLIGHT_ANIMATIONS
#define RGBLIGHT_LIMIT_VAL 120
#define RGBLIGHT_HUE_STEP 10
#define RGBLIGHT_SAT_STEP 17
#define RGBLIGHT_VAL_STEP 17

#define RGBLIGHT_SPLIT 
#define RGBLED_SPLIT { 5, 5 }


// Make sure NKRO is turned on
#undef FORCE_NKRO
#define FORCE_NKRO

// -- Leader Keys Config --
#define LEADER_PER_KEY_TIMING
#define LEADER_TIMEOUT 350  //  250

// -- One Shot Keys Config --
// #define ONESHOT_TAP_TOGGLE 2  /* Tapping this number of times holds the key until tapped once again. */
#define ONESHOT_TIMEOUT 2000  /* Time (in ms) before the one shot key is released */

// -- Auto Shift Config --
#define AUTO_SHIFT_TIMEOUT 160
#define AUTO_SHIFT_REPEAT 
// #define NO_AUTO_SHIFT_NUMERIC
 
// -- Unicode config --
#define UNICODE_SELECTED_MODES UC_WINC

// -- OLED config --
#define OLED_UPDATE_INTERVAL 50   // Sets the update rate of the OLED display in ms. Greatly improves the matrix scan rate.
                                  
// -- Debug Config --
// #define DEBUG_MATRIX_SCAN_RATE
