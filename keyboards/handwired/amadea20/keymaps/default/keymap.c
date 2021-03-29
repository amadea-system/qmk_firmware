/* Copyright 2021 Amadea System
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
#include "version.h"

#define SSDELAY 5

enum layers {
  _EMOJI_LAYER = 0,
  _FUNCTION_LAYER
};

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

    /*  Test Macros  */
    CK_TEST                // For random test stuff
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    /* Base Emoji Layer*/
    [_EMOJI_LAYER] = LAYOUT(
                            CK_HIBIKI_HAPPY,  CK_TSU_TAIL,     CK_HIBIKI_HUG,   CK_TSU_TIRED,     CK_HAPPY_YAY,
                            CK_GURA_A,        CK_GALAXY_MOON,  CK_WOONA_DANCE,  CK_WOONA_SLEEPY,  CK_LUNA_SIPPING,
                            KC_A,  KC_B,  KC_C,  KC_D,  KC_E,  
                            KC_F,  KC_G,  KC_H,  KC_I,  KC_ENTER
                            ),

    /* Function Layer */
    [_FUNCTION_LAYER] = LAYOUT(
                            KC_0,  KC_1,  KC_2,  KC_3,  KC_4,
                            KC_5,  KC_6,  KC_7,  KC_8,  KC_9,
                            KC_A,  KC_B,  KC_C,  KC_D,  KC_E,
                            KC_F,  KC_G,  KC_H,  KC_I,  KC_J
                            ),
};


bool process_record_user(uint16_t keycode, keyrecord_t *record) {

    // hid_send_activity_ping();
    // send_activity_ping = true;
    
    switch (keycode) {
        // -- Macros --
        case CK_VRSN:  // Prints out QMK Version Info
            if (record->event.pressed) {
                SEND_STRING(QMK_KEYBOARD "/" QMK_KEYMAP " @ " QMK_VERSION "\nCompiled On " QMK_BUILDDATE "\n");
            }
            break;
        
        case CK_MAKE:
            if (!record->event.pressed) {
                uint8_t temp_mod = mod_config(get_mods());
                uint8_t temp_osm = mod_config(get_oneshot_mods());
                clear_mods();
                clear_oneshot_mods();

                send_string_with_delay_P(PSTR("cp_fw && qmk"), 5);
                if ((temp_mod | temp_osm) & MOD_MASK_SHIFT){
                    send_string_with_delay_P(PSTR(" flash "), 5);
                } else {
                    send_string_with_delay_P(PSTR(" compile "), 5);
                }
                send_string_with_delay_P(PSTR("-kb " QMK_KEYBOARD " -km " QMK_KEYMAP), 5);
                send_string_with_delay_P(PSTR(SS_TAP(X_ENTER)), 5);
            }
            break;



        /*  Emoji Macros  */
        case CK_HIBIKI_HAPPY:
            if (record->event.pressed) {
                SEND_STRING_DELAY(":hibiki_happy: ", SSDELAY);
            }
            break;

        case CK_TSU_TAIL:
            if (record->event.pressed) {
                SEND_STRING_DELAY(":TsuTail: ", SSDELAY);
            }
            break;

        case CK_HIBIKI_HUG:
            if (record->event.pressed) {
                SEND_STRING_DELAY(":HibikiHug: ", SSDELAY);
            }
            break;

        case CK_TSU_TIRED:
            if (record->event.pressed) {
                SEND_STRING_DELAY(":TsuTired: ", SSDELAY);
            }
            break;

        case CK_HAPPY_YAY:
            if (record->event.pressed) {
                SEND_STRING_DELAY(":happyyay: ", SSDELAY);
            }
            break;

        case CK_GURA_A:
            if (record->event.pressed) {
                SEND_STRING_DELAY(":a_: ", SSDELAY);
            }
            break;

        case CK_GALAXY_MOON:
            if (record->event.pressed) {
                SEND_STRING_DELAY(":galaxy_moon: ", SSDELAY);
            }
            break;

        case CK_WOONA_DANCE:
            if (record->event.pressed) {
                SEND_STRING_DELAY(":woona_dance: ", SSDELAY);
            }
            break;

        case CK_WOONA_SLEEPY:
            if (record->event.pressed) {
                // SEND_STRING_DELAY(":woona_exahusted: ", SSDELAY);
                SEND_STRING_DELAY("https://i.imgur.com/vVZW8H6.jpg ", SSDELAY);
            }
            break;

        case CK_LUNA_SIPPING:
            if (record->event.pressed) {
                SEND_STRING_DELAY(":Lunasipping: ", SSDELAY);
            }
            break;


        // case SW_HIBIKI:
        //     new_fronter = MEM_HIBIKI;
        //     raw_hid_send_command(CMD_PC_SWITCH_FRONTER, &new_fronter, 1);
        //     // return false;
        //     break;
        // case SW_LUNA:
        //     new_fronter = MEM_LUNA;
        //     raw_hid_send_command(CMD_PC_SWITCH_FRONTER, &new_fronter, 1);
        //     // return false;
        //     break;
        // case CK_TEST:
        //     SEND_STRING("TEST!");
        //     // return false;
        //     break;
        // case CK_SW_TEST:
        //     SEND_STRING("TEST!");
        //     break;
    }
    return true;
}