/* 
 * This file contains code for the OLED Screens
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
#include "amadea_keymap_enums.h"


// -- Function Implementations -- //
void render_default_layer_state(void) {
    //Lines with 5 char should be _P
    //Lines with < 5char should be _ln_P

    // oled_write_ln_P(PSTR("Layer"), false);
    oled_write_P(PSTR("Layer"), false);
    oled_write_P(PSTR("  -  "), false);
    switch (get_highest_layer(layer_state)) {
        case _QWERTY:               
            oled_write_P(PSTR("QWRTY"), false);
            oled_write_ln_P(PSTR(""), false);
            break;
        case _LOWER:
            oled_write_P(PSTR("Lower"), false);
            oled_write_ln_P(PSTR(""), false);
            break;
        case _RAISE:
            oled_write_P(PSTR("Raise"), false);
            oled_write_ln_P(PSTR(""), false);
            break;
        case _ADJUST:
            oled_write_ln_P(PSTR("Mod"), false);
            oled_write_ln_P(PSTR(""), false);
            break;
        case _GAME:
            oled_write_ln_P(PSTR("Game"), false);
            oled_write_ln_P(PSTR("WASD"), true);
            break;
        case _GAME_ESDF:
            oled_write_ln_P(PSTR("Game"), false);
            oled_write_ln_P(PSTR("ESDF"), true);
            break;
        default:
            // ; //Needed because you can't declare a variable right after a label (Thanks C!!!! Hehe)
            // char undef_layer_buff[8];
            oled_write_ln_P(PSTR("Undef"), false);
            oled_write_ln_P(PSTR(""), false);
            // snprintf(undef_layer_buff, sizeof(undef_layer_buff), "%ld", layer_state);
            // oled_write(undef_layer_buff, false);
    }

    // char layer_num_buff[8];     
    // snprintf(layer_num_buff, sizeof(layer_num_buff), "%-5ld", layer_state);
    // oled_write(layer_num_buff, false);

    // Old implementation with snprintf used 1728 bytes 

    // This section: 146 Bytes
    char layer_bin_num[32] = {0};
    itoa(layer_state, layer_bin_num,2);  // 90 bytes
    strrev(layer_bin_num);  // 6 bytes

    // 42 Bytes
    for (int8_t i = 0; i < 10; i++)
    {
            if (layer_bin_num[i] == '0' || layer_bin_num[i] == '1'){
                oled_write_char(layer_bin_num[i], false);
            }else{
                oled_write_char(' ', false);
            }
    } 
}

void render_mod_status(uint8_t modifiers) {  // 108 bytes
    oled_write_ln_P(PSTR("Mods"), false);
    oled_write_P(PSTR(" "), false);
    oled_write_P(PSTR("S"), (modifiers & MOD_MASK_SHIFT));
    oled_write_P(PSTR("C"), (modifiers & MOD_MASK_CTRL));
    oled_write_P(PSTR("A"), (modifiers & MOD_MASK_ALT));
    oled_write_P(PSTR("G"), (modifiers & MOD_MASK_GUI));
}

// void render_blank_lines(int lines){

//   for (int i = 0; i < lines; i++)
//   {
//     oled_write_ln_P(PSTR(""), false);
//   }

// }