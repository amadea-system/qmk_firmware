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
#include "version.h"  // For the Version Macro
#include "nibble65_keymap.h"
#include "amadea_hid_commands.h"

// void raw_hid_send_command(uint8_t command_id, uint8_t *data, uint8_t length);
void set_rgblight_current_fronter(uint8_t current_fronter);
extern uint8_t current_fronter;

#define TAP_CODE_DELAY_M 5  

bool process_macros(uint16_t keycode, keyrecord_t *record){

    uint8_t new_fronter;  // placeholder 

    switch(keycode) {
        case CK_VRSN:  // Prints out QMK Version Info
            if (record->event.pressed) {
                SEND_STRING(QMK_KEYBOARD "/" QMK_KEYMAP " @ " QMK_VERSION "\nCompiled On " QMK_BUILDDATE "\n");
            }
            break;
        case CK_TEST:
            if (record->event.pressed) {
                SEND_STRING("Test1S");
            }
            break;            

        case CK_MAKE:  // Compiles the firmware, and adds the flash command based on keyboard bootloader
        {
            if (!record->event.pressed) {
                uint8_t temp_mod = mod_config(get_mods());
                uint8_t temp_osm = mod_config(get_oneshot_mods());
                clear_mods();
                clear_oneshot_mods();

                send_string_with_delay_P(PSTR("cp_fw && qmk"), TAP_CODE_DELAY_M);
                if ( (temp_mod | temp_osm) & (MOD_MASK_SHIFT | MOD_MASK_CTRL) ){
                    send_string_with_delay_P(PSTR(" flash "), TAP_CODE_DELAY_M);
                } else {
                    send_string_with_delay_P(PSTR(" compile "), TAP_CODE_DELAY_M);
                }
                send_string_with_delay_P(PSTR("-kb " QMK_KEYBOARD " -km " QMK_KEYMAP), TAP_CODE_DELAY_M);
                send_string_with_delay_P(PSTR(SS_TAP(X_ENTER)), TAP_CODE_DELAY_M);

                if ((temp_mod | temp_osm) & MOD_MASK_SHIFT){

                }
            }
            break;
        }
        case CK_VOL_MUTE:
            
            // uint8_t temp_osm = mod_config(get_oneshot_mods());
            if (record->event.pressed) {
                uint8_t temp_mod = mod_config(get_mods());
                // uint8_t temp_osm = mod_config(get_oneshot_mods());
                if ( (temp_mod) & (MOD_MASK_SHIFT) ){
                    send_hid_cmd_change_volume(VOLUME_MUTE_TOG, 0, VOLUME_TARGET_FOCUSED);
                }else if( (temp_mod) & (MOD_MASK_CTRL) ){
                    send_hid_cmd_get_next_volume_process(2);
                }else{
                    tap_code(KC_MUTE);
                }
            }
            break;

// ---- Layer Macros ---- //
        if (record->event.pressed) {
            case CK_QWERTY:
                rgblight_reload_from_eeprom();
                set_rgblight_current_fronter(current_fronter);
                default_layer_set(1U << _QWERTY);
                // set_single_persistent_default_layer(_QWERTY);
                break;
            case CK_COLEMAK:
                rgblight_reload_from_eeprom();
                set_rgblight_current_fronter(current_fronter);
                default_layer_set(1U << _COLEMAK);
                // set_single_persistent_default_layer(_QWERTY);
                break;
            case CK_WASD:
                rgblight_mode_noeeprom(RGBLIGHT_MODE_RAINBOW_MOOD);
                default_layer_set(1U << _GAME_WASD);
                // set_single_persistent_default_layer(_GAME_WASD);
                break;
            case CK_MINE:
                rgblight_mode_noeeprom(RGBLIGHT_MODE_RAINBOW_MOOD);
                default_layer_set(1U << _GAME_MCRAFT);
                // set_single_persistent_default_layer(_GAME_MCRAFT);
                break;
        }


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

// ---- Video Games ---- /
        case CK_MINECRAFT_AUTOPROXY:
            if(record->event.pressed){
                SEND_STRING("t" SS_DELAY(250));
                if(current_fronter == MEM_LUNA){
                    SEND_STRING("<Luna> "); 
                }else if (current_fronter == MEM_HIBIKI){
                    SEND_STRING("<Hibiki> "); 
                }else if (current_fronter == MEM_FLUTTERSHY){
                    SEND_STRING("<Fluttershy> "); 
                }
            }
            break;
    }
    return true;

}
