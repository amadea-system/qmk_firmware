/* 
 * This file contains definitions that are used in this keymap
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

#pragma once

/* ------------- Defines ------------- */
#define OLED_CHAR_WIDTH ((uint8_t)(OLED_DISPLAY_WIDTH / OLED_FONT_WIDTH))

/* ---------------- Enums -------------- */

enum custom_keycodes {
    /*  Utility Macros  */
    CK_VRSN = SAFE_RANGE,  // Prints out QMK Version Info
    CK_MAKE,  // Compiles the firmware, and adds the flash command based on keyboard bootloader
    CK_VOL_DOWN,
    CK_VOL_UP,
    CK_VOL_MUTE,

    /*  Layer Macros  */
    CK_QWERTY,
    CK_COLEMAK,
    CK_WASD,
    CK_MINE,

    /*  Switching Macros  */
    CK_SW_HIBIKI,          // Send Hibiki Switch CMD to PC
    CK_SW_LUNA,            // Send Luna  Switch CMD to PC
    CK_SW_TEST,            // Cycle through fronters internally to test code

    /*  Proxy Macros  */
    CK_PY_LUNA,
    CK_PY_FSHY,

    /*  Fun Macros  */
    CK_NYAA,

    /*  Video Game Macros  */

    // -- Mine Craft --
    CK_MINECRAFT_AUTOPROXY,

    /*  Test Macros  */
    CK_TEST                // For random test stuff
};

enum layers {
    // _BASE = 0,
    _QWERTY = 0,
    _COLEMAK,
    _GAME_WASD,
    _GAME_MCRAFT,
    _LOWER,
    _RAISE
};

// Map system members to internal ID
enum system_members {
    MEM_SWITCHED_OUT = 0,
    MEM_FLUTTERSHY,
    MEM_HIBIKI,
    MEM_LUNA
};
