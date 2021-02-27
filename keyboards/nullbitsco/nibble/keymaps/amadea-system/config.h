/* 
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

#pragma once

/* ------- Feature Configs ------- */

/* -- ARGB Underlight Config -- */
// -- Animations
#undef RGBLIGHT_ANIMATIONS
#define RGBLIGHT_EFFECT_BREATHING
#define RGBLIGHT_EFFECT_RAINBOW_MOOD
#define RGBLIGHT_EFFECT_RAINBOW_SWIRL
#define RGBLIGHT_EFFECT_SNAKE
#define RGBLIGHT_EFFECT_KNIGHT
#define RGBLIGHT_EFFECT_STATIC_GRADIENT
#define RGBLIGHT_EFFECT_ALTERNATING
#define RGBLIGHT_EFFECT_TWINKLE
// #define RGBLIGHT_EFFECT_CHRISTMAS
// #define RGBLIGHT_EFFECT_RGB_TEST

// -- Animation Settings
#define RGBLIGHT_EFFECT_BREATHE_CENTER 1.0

// -- RGB Settings
// #define RGBLIGHT_LIMIT_VAL 50


/* -- OLED config -- */
#define OLED_UPDATE_INTERVAL 50   // Sets the update rate of the OLED display in ms. Greatly improves the matrix scan rate.


/* -- NKRO Config -- */
#undef FORCE_NKRO  // Had to undef, then redefine to actually get it to activate.
#define FORCE_NKRO

/* -- Leader Keys Config -- */
#define LEADER_PER_KEY_TIMING
#define LEADER_TIMEOUT 350  //  250


/* -- Compile Space Savings Config -- */
// Saves a bunch of space (>1kb) but limits to 8/16 layers
#define LAYER_STATE_8BIT
// #define LAYER_STATE_16BIT

/* Disable unused and unneeded features to reduce on firmware size */
#ifdef LOCKING_SUPPORT_ENABLE
#    undef LOCKING_SUPPORT_ENABLE
#endif
#ifdef LOCKING_RESYNC_ENABLE
#    undef LOCKING_RESYNC_ENABLE
#endif

/* --- Nibble Features --- */
// #define NIBBLE_BIG_LED_ENABLE
// #define NIBBLE_BITC_LED_ENABLE
// #define NIBBLE_REMOTE_KB_ENABLE


/* ------- Keyboard Layouts ------- */

#define ___ KC_NO

/* This a shortcut to help you visually see your layout.
 *
 * The first section contains all of the arguments representing the physical
 * layout of the board and position of the keys.
 *
 * The second converts the arguments into a two-dimensional array which
 * represents the switch matrix.
 */

// Layout for split space mod.
#define LAYOUT_ansi_split_space( \
          K02, K03, K04, K05, K06, K07, K08, K09, K0A, K0B, K0C, K0D, K0E, K0F, K0G, \
    K11,  K12, K13, K14, K15, K16, K17, K18, K19, K1A, K1B, K1C, K1D, K1E, K1F, K1G, \
    K21,  K22, K23, K24, K25, K26, K27, K28, K29, K2A, K2B, K2C, K2D,      K2F, K2G, \
    K31,  K32,      K33, K34, K35, K36, K37, K38, K39, K3A, K3B, K3C, K3D, K3F, K3G, \
    K41,  K42, K43, K44,      K01,      K4A,      K47,      K4B, K4C, K4D, K4F, K4G  \
) { \
   {K01, K02, K03, K04, K05, K06, K07, K08, K09, K0A, K0B, K0C, K0D, K0E, K0F, K0G}, \
   {K11, K12, K13, K14, K15, K16, K17, K18, K19, K1A, K1B, K1C, K1D, K1E, K1F, K1G}, \
   {K21, K22, K23, K24, K25, K26, K27, K28, K29, K2A, K2B, K2C, K2D, ___, K2F, K2G}, \
   {K31, K32, K33, K34, K35, K36, K37, K38, K39, K3A, K3B, K3C, K3D, ___, K3F, K3G}, \
   {K41, K42, K43, K44, ___, ___, K47, ___, ___, K4A, K4B, K4C, K4D, ___, K4F, K4G}, \
}


