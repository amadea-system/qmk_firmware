
/* 
 * Leader Key Definitions
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
#include "amadea_keymap_enums.h"

extern uint8_t current_fronter;

LEADER_EXTERNS();

void matrix_scan_user(void) {
    LEADER_DICTIONARY() {
        leading = false;
        leader_end();


        /* --- In-Use Key Combos ---
         *
         * - Single Keys - 
         * Q, R, L, F, G, 1, N, B, Backspace
         * 
         * - Double Keys - 
         * E-T, E-H, E-S, E-D, S-T, R-T, C-N
         *
         * - Tripple Keys - 
         * A-S-T
         * 
         */
 
        // -- Unicode Characters -- //
        SEQ_ONE_KEY(KC_Q) { send_unicode_string("‽");}  //Sens the Introbang (‽) Characters

        // -- Proxy Tags -- //
        SEQ_ONE_KEY(KC_R) { SEND_STRING("Luna\\ "); }           //Luna
        SEQ_ONE_KEY(KC_L) { SEND_STRING(":crescent_moon: "); }  //Luna
        SEQ_ONE_KEY(KC_F) { SEND_STRING("/f "); }               //Fluttershy

        // Autoproxy
        SEQ_ONE_KEY(KC_G) {
            if(current_fronter == MEM_LUNA){
                SEND_STRING("Luna\\ "); 
            }else if (current_fronter == MEM_HIBIKI){
                SEND_STRING("[ "); 
            }else if (current_fronter == MEM_FLUTTERSHY){
                SEND_STRING("/f "); 
            }
        }  


        // -- Fun Stuff!!!!!!!!!! -- //
        // !!!!!!!!!!!!!!!!!!!!!!
        SEQ_ONE_KEY(KC_1) { SEND_STRING("!!!!!!!!!!"); }             // !!!!!!!!!!!!!!!!!!!!
        SEQ_ONE_KEY(KC_N) { SEND_STRING("Nyaaaaaa!!!!!"); }          // Nyaaaa

        // -- Discord Emoji  -- //
        SEQ_TWO_KEYS(KC_E, KC_T) { SEND_STRING(":TsuTail:"); }       // TsuTail
        SEQ_TWO_KEYS(KC_E, KC_H) { SEND_STRING(":hibiki_happy:"); }  // Hibiki_Happy
        SEQ_TWO_KEYS(KC_E, KC_S) { SEND_STRING(":TsuTired:"); }      // TsuTired
        SEQ_TWO_KEYS(KC_E, KC_D) { SEND_STRING(":dabmikudab:"); }    // Dabbing Miku

        // -- Images -- //

        SEQ_TWO_KEYS(KC_S, KC_T) {
            if(current_fronter == MEM_LUNA){
                SEND_STRING("Luna\\ https://i.imgur.com/vVZW8H6.jpg"); // Exhausted Woona
            }else{
                SEND_STRING(":TsuTired:");
            }
        }     

        // -- Web Browsers -- //
        // SEQ_ONE_KEY(KC_BSPC) { SEND_STRING(SS_TAP(X_WBAK)); }         // Go back
        SEQ_ONE_KEY(KC_B) { SEND_STRING(SS_TAP(X_WBAK)); }               // Go back
        SEQ_TWO_KEYS(KC_R, KC_T) {SEND_STRING(SS_LCTRL(SS_LSFT("t")));}  // Re-opens closed tab (CTRL+Shift+T)
        SEQ_TWO_KEYS(KC_C, KC_N) {SEND_STRING(SS_LCTL("c") SS_DELAY(250) SS_LCTL("t") SS_DELAY(250) SS_LCTL("v") SS_DELAY(250) SS_TAP(X_ENTER) );}  // Copies, Pastes, Open's in new tab (CTRL+Shift+T)

        // -- Other -- //
        SEQ_ONE_KEY(KC_BSPC) {
            SEND_STRING(SS_LCTL(SS_LSFT(SS_TAP(X_LEFT)))SS_TAP(X_BSPC));  // Delete Word
        }

        // -- QMK Features -- //

        // Autoshift
        #ifdef AUTO_SHIFT_ENABLE
        SEQ_THREE_KEYS(KC_A, KC_S, KC_T) { autoshift_toggle(); }  // Toggle
        #endif

        // Toggle interactive terminal
        // SEQ_TWO_KEYS(KC_T, KC_O) { SEND_STRING()}

    }
}