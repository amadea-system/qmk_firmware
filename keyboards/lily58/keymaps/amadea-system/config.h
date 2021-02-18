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

// -- USB Ident Config -- 
#undef MANUFACTURER
#define MANUFACTURER Amadea

// #undef VENDOR_ID   
// #define VENDOR_ID       0x04D9

// #undef PRODUCT_ID  
// #define PRODUCT_ID      0xEB2E

// #undef DEVICE_VER
// #define DEVICE_VER      0x0100


// -- General Config --

/* Select hand configuration */
#define MASTER_LEFT
// #define MASTER_RIGHT
// #define EE_HANDS

// #define USE_SERIAL_PD2  // I don't think this does anything...

// -- Compile Space Savings Config --
// Saves a bunch of space (>1kb) but limits to 8/16 layers
// #define LAYER_STATE_8BIT
#define LAYER_STATE_16BIT


// -- RGB Underglow Config --

#define SPLIT_RGB_PIXEL_CONTROL
#undef RGBLED_NUM
#define RGBLED_NUM 12
#define RGBLED_SPLIT { 6, 6 }
#define RGBLIGHT_LED_MAP { 5, 4, 3, 2, 1, 0, 6, 7, 8, 9, 10}

#define RGBLIGHT_SPLIT 
#define RGBLIGHT_LIMIT_VAL 200 //120
#define RGBLIGHT_HUE_STEP 10
#define RGBLIGHT_SAT_STEP 17
#define RGBLIGHT_VAL_STEP 17

// - RGB Animation Settings -
#undef RGBLIGHT_ANIMATIONS
#define RGBLIGHT_EFFECT_BREATHING  // 930 Bytes
// #define RGBLIGHT_EFFECT_BREATHE_CENTER 1.0  // 1558 Bytes

// -- NKRO Config --
#undef FORCE_NKRO  // Had to undef, then redefine to actually get it to activate.
#define FORCE_NKRO


// -- Tap-Hold Config --
// https://docs.qmk.fm/#/tap_hold
#define TAPPING_TERM 200    // How long till a keypress is considered a 'hold'  //100 
#define TAPPING_FORCE_HOLD  // This will prevent ONESHOT_TAP_TOGGLE from functioning.



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

// -- Combo Config --
#define COMBO_TERM 300  // Sets the time out period for combos.

// -- Debug Config --
// #define DEBUG_MATRIX_SCAN_RATE


/* Disable unused and unneeded features to reduce on firmware size */
#ifdef LOCKING_SUPPORT_ENABLE
#    undef LOCKING_SUPPORT_ENABLE
#endif
#ifdef LOCKING_RESYNC_ENABLE
#    undef LOCKING_RESYNC_ENABLE
#endif