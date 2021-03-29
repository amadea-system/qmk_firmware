/* 
 * This file contains definitions that are used in this keymap
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

#pragma once

/* ------------- Defines ------------- */

/* ---------------- Enums -------------- */

enum custom_keycodes {
    /*  Utility Macros  */
    CK_VRSN = SAFE_RANGE,  // Prints out QMK Version Info

    CK_MAKE,  // Compiles the firmware, and adds the flash command based on keyboard bootloader

    /*  Layer Macros  */
    CK_EMOJI_LY,
    CK_FUNC_LY,

    /*  Switching Macros  */
    CK_SW_HIBIKI,          // Send Hibiki Switch CMD to PC
    CK_SW_LUNA,            // Send Luna  Switch CMD to PC
    CK_SW_TEST,            // Cycle through fronters internally to test code

    /*  Proxy Macros  */
    CK_PY_LUNA,
    CK_PY_FSHY,

    /*  Emoji Macros  */
    CK_HIBIKI_HAPPY,
    CK_TSU_TAIL,
    CK_HIBIKI_HUG,
    CK_TSU_TIRED,
    CK_HAPPY_YAY,
    CK_GURA_A,
    CK_GALAXY_MOON,
    CK_WOONA_DANCE,
    CK_WOONA_SLEEPY,
    CK_LUNA_SIPPING,

    CK_MEOW_WAVE,
    CK_DAB_MIKU,
    CK_WOONA,
    CK_WOONA_STARE,
    CK_LUNA_SHRUG,
    CK_LUNA_SMILE,


    /*  Test Macros  */
    CK_TEST                // For random test stuff
};

// Map system members to internal ID
enum system_members {
    MEM_SWITCHED_OUT = 0,
    MEM_FLUTTERSHY,
    MEM_HIBIKI,
    MEM_LUNA
};
