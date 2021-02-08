/* 
 * Macro Definitions
 * 
 * https://docs.qmk.fm/#/feature_macros
 * https://thomasbaart.nl/2018/12/20/qmk-basics-leader-key/
 * https://tulpa.dev/cadey/kadis-layouts/src/branch/master/moonlander/leader.c
 * 
 * ------------------------------------------------------------------------------ 
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


#include "quantum.h"
#include "nibble65_keymap_enums.h"

void raw_hid_send_command(uint8_t command_id, uint8_t *data, uint8_t length);

bool process_macros(uint16_t keycode, keyrecord_t *record){

    uint8_t new_fronter;  // placeholder 
    switch(keycode) {
        case CK_VRSN:  // Prints out QMK Version Info
            if (record->event.pressed) {
                SEND_STRING(QMK_KEYBOARD "/" QMK_KEYMAP " @ " QMK_VERSION "\nCompiled On " QMK_BUILDDATE "\n");
            }
            break;

// ---- Switching Macros ---- //
        case CK_SW_HIBIKI:
            if (record->event.pressed) {
                new_fronter = MEM_HIBIKI;
                raw_hid_send_command(CMD_PC_SWITCH_FRONTER, &new_fronter, 1);
            }
            break;

        case CK_SW_LUNA:
            if (record->event.pressed) {
                new_fronter = MEM_LUNA;
                raw_hid_send_command(CMD_PC_SWITCH_FRONTER, &new_fronter, 1);
            }
            break;

// ---- Fun Stuff ---- //
        case CK_NYAA:  // Nyaaaaaa!!!!!
            if (record->event.pressed) {
                SEND_STRING("Nyaaaaaa!!!!!");
            }
            break;

// ---- Proxy Tags ---- //
        case CK_PY_LUNA:  // Nyaaaaaa!!!!!
            if (record->event.pressed) {
                SEND_STRING("Luna\\ ");
            }
            break;

        case CK_PY_FSHY:  // Nyaaaaaa!!!!!
            if (record->event.pressed) {
                SEND_STRING("/f ");
            }
            break;

        // case RM_4: //remote macro 4
        //   if (record->event.pressed) {
        //   }
        // break;
    }
    return true;

}
