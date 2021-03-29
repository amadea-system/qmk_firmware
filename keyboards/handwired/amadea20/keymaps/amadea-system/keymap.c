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
// #include "amadea20_hid_commands.h"
// #include "amadea20_hid_commands.c"
#include "version.h"

#include "amadea20_common.h"


#define SSDELAY 5

enum layers {
  _EMOJI_LAYER = 0,
  _FUNCTION_LAYER
};


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    /* Base Emoji Layer*/
    [_EMOJI_LAYER] = LAYOUT(
                            CK_HIBIKI_HAPPY,  CK_TSU_TAIL,   CK_HIBIKI_HUG,   CK_TSU_TIRED,     CK_HAPPY_YAY,
                            CK_GURA_A,        CK_MEOW_WAVE,  CK_DAB_MIKU,     CK_WOONA_SLEEPY,  CK_LUNA_SIPPING,
                            CK_WOONA_DANCE,   CK_WOONA,      CK_WOONA_STARE,  CK_LUNA_SHRUG,    CK_LUNA_SMILE,  
                            CK_GALAXY_MOON,   RGB_MODE_FORWARD,         KC_NO,           RGB_MODE_REVERSE,            KC_ENTER
                            ),

    /* Function Layer */
    [_FUNCTION_LAYER] = LAYOUT(
                            KC_0,  KC_1,  KC_2,  KC_3,  KC_4,
                            KC_5,  KC_6,  KC_7,  KC_8,  KC_9,
                            KC_A,  KC_B,  KC_C,  KC_D,  KC_E,
                            KC_F,  KC_G,  KC_H,  KC_I,  KC_J
                            ),
};

led_config_t g_led_config = { {
  // Key Matrix to LED Index
  { NO_LED, NO_LED, 15, 10, 9 },
  { 0,      16,     14, 11, 8 },
  { 1,      17,     13, 12, 7 },
  { 2,      3,      4,  5,  6 }
}, {
  // LED Index to Physical Position
  {0, 21}, {0, 43}, {0, 64}, {56, 64}, {112, 64}, {168, 64}, {224, 64}, {224, 43}, {224, 21}, {224, 0}, {168, 0}, {168, 21}, {168, 43}, {112, 43}, {112, 21}, {112, 0}, {56, 21}, {56, 43}
}, {
  // LED Index to Flag
   4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 
} };


void keyboard_post_init_user(void) {

   // Call the post init code.

    // #ifdef RGBLIGHT_ENABLE
        // rgblight_setrgb_range(0.5, 0.5, 0.5, 0, 5);
        // rgblight_enable_noeeprom(); // Enables RGB, without saving settings
        // rgblight_disable_noeeprom();
        rgblight_disable();
        

        // rgblight_sethsv_noeeprom(DEFAULT_ARGB_COLOR);
        // rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);

        // rgblight_sethsv_noeeprom(rgblight_get_hue(), rgblight_get_sat(), 50);
        // rgblight_sethsv(191,255, 50);
        // rgblight_sethsv(HSV_WHITE);
        // rgblight_sethsv(255, 255, 100);

        rgb_matrix_enable();
        rgb_matrix_sethsv(234, 128, 100);

        // rgblight_mode_noeeprom(RGBLIGHT_MODE_RAINBOW_MOOD);
        
        
    // #endif

    // uint8_t data = 42;
    // raw_hid_send_command(CMD_PC_RAW_DEBUG_MSG, &data, sizeof(data));
}


// void matrix_scan_user(void) {
//     check_hid_timeout();
// }


// layer_state_t layer_state_set_user(layer_state_t state) {

//     // Activate ADJUST layer when LOWER & UPPER are active
//     // state = update_tri_layer_state(state, _RAISE, _LOWER, _ADJUST);

//     // Send notification MSG to connected PC of layer state
//     raw_hid_send_command(CMD_PC_NOTIFY_LAYER_CHANGE, (uint8_t *)&state, sizeof(state));
//     return state;
// }


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

        case CK_MEOW_WAVE:
            if (record->event.pressed) {
                SEND_STRING_DELAY(":ameowwave: ", SSDELAY);
            }
            break;

        case CK_DAB_MIKU:
            if (record->event.pressed) {
                SEND_STRING_DELAY(":dabmikudab: ", SSDELAY);
            }
            break;

        case CK_WOONA:
            if (record->event.pressed) {
                SEND_STRING_DELAY(":Woona: ", SSDELAY);
            }
            break;

        case CK_WOONA_STARE:
            if (record->event.pressed) {
                SEND_STRING_DELAY(":WoonaStare: ", SSDELAY);
            }
            break;

        case CK_LUNA_SHRUG:
            if (record->event.pressed) {
                SEND_STRING_DELAY(":Luna_shrug: ", SSDELAY);
            }
            break;

        case CK_LUNA_SMILE:
            if (record->event.pressed) {
                SEND_STRING_DELAY(":LunaSmile: ", SSDELAY);
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