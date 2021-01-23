/* 
 * This file contains enums that are used in this keymap
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

//tap dance states
enum tap_dance_state {
    SINGLE_TAP = 1,
    SINGLE_HOLD = 2,
    DOUBLE_TAP = 3,
    TRIPLE_TAP = 4,
    TRIPLE_HOLD = 5
};

//tap dance keys
enum tap_dance_keycodes{
    TAPPY_KEY = 0
};

enum system_members {
    MEM_SWITCHED_OUT = 0,
    MEM_FLUTTERSHY,
    MEM_HIBIKI,
    MEM_LUNA
};


enum hid_commands {
    CMD_DO_NOTHING = 0,  // Do Nothing

    // CMDs Sent From PC
    CMD_KB_SET_CURRENT_FRONTER = 1,
    CMD_KB_SET_ALL_RGB_LEDS,
    CMD_KB_SET_RGB_LEDS,

    // CMDs Sent To PC
    CMD_PC_RAW_DEBUG_MSG = 120,
    CMD_PC_DEBUG_MSG,
    CMD_PC_SWITCH_FRONTER,
    CMD_PC_NOTIFY_LAYER_CHANGE
};
