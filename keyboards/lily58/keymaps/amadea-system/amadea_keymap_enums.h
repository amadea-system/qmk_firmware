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

enum layer_number {
  _QWERTY = 0,
  _LOWER,
  _RAISE,
  _ADJUST,
  _GAME_WASD,
  _GAME_ESDF,
};


enum custom_keycodes {
    VRSN = SAFE_RANGE,  // Prints out QMK Version Info
    SW_HIBIKI,
    SW_LUNA
};


enum custom_tap_dance_keycodes {
    TD_COMMA = 0,   // Left Brackets
    TD_DOT,         // Right Brackets
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

  // CMDs Sent To PC
  CMD_PC_SWITCH_FRONTER = 120,  // Use PK API To switch fronters
  CMD_PC_NOTIFY_LAYER_CHANGE = 121  // Notifiy the PC that the current active layer has changed
};
