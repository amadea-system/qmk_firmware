/* 
 * Nibble 65% OLED Display Code 
 * 
 * ----------------------------------------------------------------------
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
 * 
 */

#include "quantum.h"
#include "nibble65_keymap.h"

/* --------- External Variables --------- */
extern uint8_t current_fronter;
extern uint32_t last_led_changed_time;


/* ------------ Function Defs ----------- */

void render_top_header(void){
    oled_write_P(PSTR("_Layer_"), false);    // 7 Chars
    oled_write_P(PSTR("     "), false);      // 5 Chars
    oled_write_P(PSTR("_Fronter_"), false);  // 9 Chars
}


void render_layer_state(void) {
    /* 6 char long */

    // Layer name must be 6 characters long.
    switch (get_highest_layer(layer_state)) {
        case _QWERTY:               
            oled_write_P(PSTR("QWERTY"), false);
            break;
        case _LOWER:
            oled_write_P(PSTR("Lower "), false);
            break;
        case _RAISE:
            oled_write_P(PSTR("Macros"), false);
            break;
        default:
            oled_write_P(PSTR("Undef "), false);
    }
}

void render_current_fronter(void){
    /* 6 char long */

    if (check_hid_timeout()){
        switch (current_fronter) {
            case MEM_SWITCHED_OUT:
                oled_write_P(PSTR("  Sleep"), false);
                break;
            case MEM_FLUTTERSHY:
                oled_write_P(PSTR("Flutter"), false);
                break;
            case MEM_HIBIKI:
                oled_write_P(PSTR(" Hibiki"), false);
                break;
            case MEM_LUNA:
                oled_write_P(PSTR("   Luna"), false);
                break;
            case 255:
                oled_write_P(PSTR("  Error"), false);
                break;
        }
    }else{
      oled_write_P(PSTR(" Discon"), false);
    }
}

void render_layer_and_fronter(void){

    render_layer_state();     // 6 char
    oled_write_P(PSTR("        "), false);      // 8 Chars
    render_current_fronter(); // 7 char

}

// extern rgblight_config_t rgblight_config;
// void render_rgb_status(void){

//     oled_write("", false);

//   snprintf(rbf_info_str, sizeof(rbf_info_str), "%s %2d h%3d s%3d v%3d",
//     rgblight_config.enable ? "on" : "- ", rgblight_config.mode,
//     rgblight_config.hue, rgblight_config.sat, rgblight_config.val);
//   return rbf_info_str;
// }


extern rgblight_config_t rgblight_config;
void render_rgb_state(void) {
    #ifdef RGBLIGHT_ENABLE
    
    // if(timer_expired(timer_read(), last_led_changed_time + 15000)){
    if (timer_elapsed32(last_led_changed_time) > 15000) {
        // Only show the RGB Info for a short time after changing it.
        oled_write_ln_P(PSTR(""), false);
        return;
    }
    char rgb_info_str[22];

        // snprintf(rgb_info_str, sizeof(rgb_info_str), "RGB %2d H%3d S%3d V%3d",//" LED %2d: %3d,%3d,%3d ",
        //      rgblight_get_mode(),
        //      (uint8_t)((rgblight_get_hue()*100)/255),
        //      (uint8_t)((rgblight_get_sat()*100)/255),
        //      (uint8_t)((rgblight_get_val()*100)/255));

    snprintf(rgb_info_str, sizeof(rgb_info_str), "RGB %2d H%3d S%3d V%3d",   //"%s %2d h%3d s%3d v%3d",
            //  rgblight_config.enable ? "on" : "- ",
             rgblight_config.mode,
             (uint8_t)((rgblight_config.hue*100)/255), 
             (uint8_t)((rgblight_config.sat*100)/255), 
             (uint8_t)((rgblight_config.val*100)/255));

    oled_write(rgb_info_str, false);
    #endif // RGBLIGHT_ENABLE
}
