/* 
 * This file contains the primary keymap code for the Lily58
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
 * 
 *  --- Notes ---
 * How to disable the "Office" key from blocking the Hyper Key
 * https://superuser.com/questions/1457073/how-do-i-disable-specific-windows-10-office-keyboard-shortcut-ctrlshiftwinal
 * https://www.howtogeek.com/445318/how-to-remap-the-office-key-on-your-keyboard/
 * 
 * 
 */


#include QMK_KEYBOARD_H

#ifdef VSCODE_WORKAROUND
  #include "config.h"
  #include "keyboards/lily58/rev1/config.h"
#endif

// #ifdef PROTOCOL_LUFA
//   #include "lufa.h"
//   #include "split_util.h"
// #endif
// #ifdef SSD1306OLED
//   #include "ssd1306.h"
// #endif

#include "amadea_keymap_enums.h"
#include "version.h"  // For the Version Macro
#include "raw_hid.h"
#include "custom_transport/rgblight_user.h"



extern uint8_t is_master;

// HID Vars
uint8_t current_fronter = MEM_SWITCHED_OUT;
bool hid_connected = false; // Flag indicating if we have a PC connection yet
static uint16_t hid_disconection_timer = 0;
static uint16_t activity_ping_timer = 0;

bool send_activity_ping = false;
static uint16_t last_hid_transmition_time = 0;

/* ----------- Function Defs --------- */
void process_leader_key_user(void);
void hid_send_activity_ping(void);

// -----------


#ifdef UNICODEMAP_ENABLE
  // --- Unicode Map --- //
  enum unicode_names {
      MOON,
      BUTTERFLY,
      SMILE_CAT,
      INTROBANG
  };

  const uint32_t PROGMEM unicode_map[] = {
      [MOON]      = 0x1F319, //🌙
      [BUTTERFLY] = 0x1F98B, //🦋
      [SMILE_CAT] = 0x1F63A, //😺
      [INTROBANG] = 0x203D,  // ‽
  };


  #define E_MOON X(MOON)
  #define E_BFLY X(BUTTERFLY)
  #define E_S_CAT X(SMILE_CAT)
  #define E_INTRO X(INTROBANG)
#else
  // #ifdef UNICODE_ENABLE
    #define E_MOON XXXXXXX
    #define E_BFLY XXXXXXX
    #define E_S_CAT XXXXXXX
    #define E_INTRO XXXXXXX
  // #endif
#endif

// ------------------- //



const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* QWERTY
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * | ESC  |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  |  `   |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Tab  |   Q  |   W  |   E  |   R  |   T  |                    |   Y  |   U  |   I  |   O  |   P  |  -   |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |LShift|   A  |   S  |   D  |   F  |   G  |-------.    ,-------|   H  |   J  |   K  |   L  |   ;  |  '   |
 * |------+------+------+------+------+------| Leader|    | Hyper |------+------+------+------+------+------|
 * |LCTRL |   Z  |   X  |   C  |   V  |   B  |-------|    |-------|   N  |   M  |   ,  |   .  |   /  |RShift|
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *                   | LAlt | LGUI |LOWER | /Space  /       \Enter \  |RAISE |BackSP| RGUI |
 *                   |      |      |      |/       /         \      \ |      |      |      |
 *                   `----------------------------'           '------''--------------------'
 */
/*
 [_QWERTY] = LAYOUT( \
  KC_ESC,   KC_1,   KC_2,    KC_3,    KC_4,    KC_5,                     KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_GRV, \
  KC_TAB,   KC_Q,   KC_W,    KC_E,    KC_R,    KC_T,                     KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_MINS, \
  KC_LCTRL, KC_A,   KC_S,    KC_D,    KC_F,    KC_G,                     KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, \
  KC_LSFT,  KC_Z,   KC_X,    KC_C,    KC_V,    KC_B, KC_LBRC,  KC_RBRC,  KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,  KC_RSFT, \
                        KC_LALT, KC_LGUI, MO(_LOWER), KC_SPC, KC_ENT, MO(_RAISE), KC_BSPC, KC_RGUI \
),
*/
#define OSM_LSFT OSM(MOD_LSFT)
// #define TD_MINS  TD(TD_MINUS_EQL)      // Send '-' (Minus Key) on a Tap, '=' (Equal Key) on a Double Tap, 
// #define MT_LEAD  HYPR_T(KC_LEAD)   // Left Control, Shift, Alt and GUI when held, 'Leader Key' when tapped 

 [_QWERTY] = LAYOUT( \
  KC_ESC,   KC_1,   KC_2,    KC_3,    KC_4,    KC_5,                        KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_GRV, \
  KC_TAB,   KC_Q,   KC_W,    KC_E,    KC_R,    KC_T,                        KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_MINS, \
  OSM_LSFT, KC_A,   KC_S,    KC_D,    KC_F,    KC_G,                        KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, \
  KC_LCTRL, KC_Z,   KC_X,    KC_C,    KC_V,    KC_B,  KC_LEAD,   KC_F19,    KC_N,    KC_M,    KC_COMMA, KC_DOT, KC_SLSH, KC_EQL, \
                        KC_LALT, KC_LGUI, MO(_LOWER), KC_SPC,    KC_ENT, MO(_RAISE), KC_BSPC, KC_RGUI \
),

/* LOWER - Acessed by pressing 'Lower' Key
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |  F12 |  F1  |  F2  |  F3  |  F4  |  F5  |                    |  F6  |  F7  |  F8  |  F9  | F10  | F11  |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |      |Pg Up |  Up  |Pg Dwn|   {  |                    |      |   }  |      |      |      |   =  |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |   !  | Left | Down |Right |   %  |-------.    ,-------|   ^  |   &  |   *  |      |   |  |      |
 * |------+------+------+------+------+------|   [   |    |    ]yr  |------+------+------+------+------+------|
 * |      |      |      |      |      |      |-------|    |-------|      |   _  |  [/{ |  ]/} |   \  |RShift|
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *                   | LAlt | LGUI |LOWER | /Space  /       \Enter \  |RAISE |BackSP| RGUI |
 *                   |      |      |      |/       /         \      \ |      |      |      |
 *                   `----------------------------'           '------''--------------------'
 */

/*
[_LOWER] = LAYOUT( \
  _______, _______, _______, _______, _______, _______,                   _______, _______, _______,_______, _______, _______,\
  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,                     KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12, \
  KC_GRV, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC,                   KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_TILD, \
  _______, _______, _______, _______, _______, _______, _______, _______, XXXXXXX, KC_UNDS, KC_PLUS, KC_LCBR, KC_RCBR, KC_PIPE, \
                             _______, _______, _______, _______, _______,  _______, _______, _______\
),
*/

[_LOWER] = LAYOUT( \
  KC_F12,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                     KC_F6,   KC_F7,   KC_F8,       KC_F9,       KC_F10,  KC_F11,  \
  KC_NO,   KC_NO,   KC_PGUP, KC_UP,   KC_PGDN, KC_PGUP,                   KC_NO,   KC_RCBR, KC_NO,       KC_NO,       KC_NO,   KC_EQL,  \
  _______, KC_EXLM, KC_LEFT, KC_DOWN, KC_RGHT, KC_PGDN,                   KC_CIRC, KC_AMPR, KC_ASTR,     KC_NO,       KC_PIPE, KC_NO,   \
  _______, _______, _______, _______, _______, _______, KC_RBRC, _______, XXXXXXX, KC_UNDS, TD(TD_LBRK), TD(TD_RBRK), KC_BSLS, _______, \
                             _______, _______, _______, _______, _______,  _______, _______, _______\
),

// [_LOWER] = LAYOUT( 
//       ,   ,  ,  ,  ,   ,            
//       KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,     KC_TRNS, KC_TRNS,     KC_NO, KC_UNDS, KC_LBRC, KC_RBRC, KC_BSLS, KC_TRNS, 
//       KC_TRNS, KC_TRNS, KC_TRNS,                                KC_TRNS, KC_TRNS,     MO(3), KC_DEL, KC_TRNS
// ),


/* RAISE - Acessed by pressing 'Raise' Key
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |      |  ‽   |  🌙  |  🦋 |  😺 |      |                    |      |      |      |      |      |Vol Up|
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |      |      |      |      |      |                    |      |      |GUI Up|      |      |Vol Dw|
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |-------.    ,-------|      |GUI Lf|GUI Dw|GUI Rt|      | Mute |
 * |------+------+------+------+------+------|   [   |    |    ]  |------+------+------+------+------+------|
 * |  F7  |  F8  |  F9  | F10  | F11  | F12  |-------|    |-------|   +  |   -  |   =  |   [  |   ]  |   \  |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *                   | LAlt | LGUI |GAME1 | /Space  /       \Enter \  |RAISE | Del  | RGUI |
 *                   |      |      |      |/       /         \      \ |      |      |      |
 *                   `----------------------------'           '------''--------------------'
 */

/*
[_RAISE] = LAYOUT( \
  _______, _______, _______, _______, _______, _______,                     _______, _______, _______, _______, _______, _______, \
  KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                        KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    _______, \
  KC_F1,  KC_F2,    KC_F3,   KC_F4,   KC_F5,   KC_F6,                       XXXXXXX, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, XXXXXXX, \
  KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,   _______, _______,  KC_PLUS, KC_MINS, KC_EQL,  KC_LBRC, KC_RBRC, KC_BSLS, \
                             _______, _______, _______,  _______, _______,  _______, _______, _______ \
),
*/

[_RAISE] = LAYOUT( \
  XXXXXXX,  E_INTRO,  E_MOON,  E_BFLY,  E_S_CAT, XXXXXXX,                     XXXXXXX, XXXXXXX,       XXXXXXX,       XXXXXXX,       XXXXXXX,  KC_AUDIO_VOL_UP, \
  XXXXXXX,  XXXXXXX,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                     XXXXXXX, XXXXXXX,       LGUI(KC_UP),   XXXXXXX,       XXXXXXX,  KC_AUDIO_VOL_DOWN, \
  KC_F1,    KC_F2,    KC_F3,   KC_F4,   KC_F5,   KC_F6,                       XXXXXXX, LGUI(KC_LEFT), LGUI(KC_DOWN), LGUI(KC_RGHT), XXXXXXX,  KC_AUDIO_MUTE, \
  KC_F7,    KC_F8,    KC_F9,   KC_F10,  KC_F11,  KC_F12,   _______, _______,  KC_PLUS, KC_MINS,       KC_EQL,        KC_LBRC,       KC_RBRC,  KC_BSLS, \
                               _______, _______, _______,  _______, _______,  _______, KC_DEL,        _______ \
),

// [2] = LAYOUT(KC_NO, KC_NO, KC_NO, KC_NO,  KC_NO,  KC_NO,                      KC_NO,   KC_NO,         KC_NO,         KC_NO,         KC_NO,   KC_VOLU, 
//              KC_NO, KC_NO, KC_NO, KC_NO,  KC_NO,  KC_NO,                      KC_NO,   KC_NO,         LGUI(KC_UP),   KC_NO,         KC_NO,   KC_VOLD, 
//              KC_F1, KC_F2, KC_F3, KC_F4,  KC_F5,  KC_F6,                      KC_NO,   LGUI(KC_LEFT), LGUI(KC_DOWN), LGUI(KC_RGHT), KC_NO,   KC_MUTE, 
//              KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, KC_TRNS, KC_TRNS,   KC_PLUS, KC_MINS,       KC_EQL,        KC_LBRC,       KC_RBRC, KC_BSLS,
//                                   KC_TRNS, KC_TRNS, MO(3), KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS,       KC_TRNS
//                                   ),


/* MACRO - Acessed by pressing 'Macro'
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |Versio|      |L-WASD|L-ESDF|      |      |                    |SW_Hib|SW_Lun|      |      |      |RESET |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |      |      |      |      |      |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |      |      |      |      |      |-------.    ,-------|      |      |RGB ON| HUE+ | SAT+ | VAL+ |
 * |------+------+------+------+------+------|       |    |       |------+------+------+------+------+------|
 * |      |      |      |      |      |      |-------|    |-------|      |      | MODE | HUE- | SAT- | VAL- |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *                   | LAlt | LGUI |LOWER | /Space  /       \Enter \  |RAISE |BackSP| RGUI |
 *                   |      |      |      |/       /         \      \ |      |      |      |
 *                   `----------------------------'           '------''--------------------'
 */

/*
[_MACRO] = LAYOUT( \
XXXXXXX, XXXXXXX, XXXXXXX,  XXXXXXX,  XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
XXXXXXX, XXXXXXX, XXXXXXX,  XXXXXXX,  XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
XXXXXXX, XXXXXXX, XXXXXXX,  XXXXXXX,  XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
XXXXXXX, XXXXXXX, XXXXXXX,  XXXXXXX,  XXXXXXX, XXXXXXX, _______, _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
                            _______,  _______, _______, _______, _______,  _______, _______, _______ \
),
*/

/* ADJUST - Acessed by pressing 'Lower' & 'Raise' Keys
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |Versio|      |L-WASD|L-ESDF|      |SWTest|                    |SW_Hib|SW_Lun|      |      |      |RESET |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |      |      |      |      | Test |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |      |      |      |      |      |-------.    ,-------|      |      |RGB ON| HUE+ | SAT+ | VAL+ |
 * |------+------+------+------+------+------|       |    |       |------+------+------+------+------+------|
 * |      |      |      |      |      |      |-------|    |-------|      |      | MODE | HUE- | SAT- | VAL- |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *                   | LAlt | LGUI |LOWER | /Space  /       \Enter \  |RAISE |BackSP| RGUI |
 *                   |      |      |      |/       /         \      \ |      |      |      |
 *                   `----------------------------'           '------''--------------------'
 */

#define TOG_ESDF TG(_GAME_ESDF)
#define TOG_WASD TG(_GAME_WASD)

[_ADJUST] = LAYOUT( \
VRSN,    XXXXXXX, TOG_WASD, TOG_ESDF, XXXXXXX, CK_SW_TEST,              SW_HIBIKI, SW_LUNA, XXXXXXX, XXXXXXX, XXXXXXX, RESET,   \
XXXXXXX, XXXXXXX, XXXXXXX,  XXXXXXX,  XXXXXXX, CK_TEST,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
XXXXXXX, XXXXXXX, XXXXXXX,  XXXXXXX,  XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, RGB_TOG, RGB_HUI, RGB_SAI, RGB_VAI, \
XXXXXXX, XXXXXXX, XXXXXXX,  XXXXXXX,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, RGB_MOD, RGB_HUD, RGB_SAD, RGB_VAD, \
                            _______,  _______, _______, _______, _______,  _______, _______, _______ \
),


/* Game (QWERTY) - Acessed by pressing 'L-WASD' Key in the Adjust Layer
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * | ESC  |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  |  `   |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Tab  |   Q  |   W  |   E  |   R  |   T  |                    |   Y  |   U  |   I  |   O  |   P  |  -   |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |LShift|   A  |   S  |   D  |   F  |   G  |-------.    ,-------|   H  |   J  |   K  |   L  |   ;  |  '   |
 * |------+------+------+------+------+------|   [   |    |    ]  |------+------+------+------+------+------|
 * |LCTRL |   Z  |   X  |   C  |   V  |   B  |-------|    |-------|   N  |   M  |   ,  |   .  |   /  |RShift|
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *                   | LAlt | SPACE|LOWER | /Space  /       \Enter \  |RAISE |BackSP| SPACE|
 *                   |      |      |      |/       /         \      \ |      |      |      |
 *                   `----------------------------'           '------''--------------------'
 */

[_GAME_WASD] = LAYOUT( \
KC_ESC,   KC_1,   KC_2,    KC_3,    KC_4,    KC_5,                    KC_6,     KC_7,    KC_8,    KC_9,    KC_0,    KC_GRV, \
KC_TAB,   KC_Q,   KC_W,    KC_E,    KC_R,    KC_T,                    KC_Y,     KC_U,    KC_I,    KC_O,    KC_P,    KC_MINS, \
KC_LSFT,  KC_A,   KC_S,    KC_D,    KC_F,    KC_G,                    KC_H,     KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, \
KC_LCTRL, KC_Z,   KC_X,    KC_C,    KC_V,    KC_B,  KC_LEAD, KC_SPC,  KC_N,     KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, \
                           KC_LALT, KC_SPC, KC_SPC, KC_SPC,  KC_ENT,  TOG_WASD, KC_BSPC, KC_RGUI \
),


/* Game (ESDF) - Acessed by pressing 'L-ESDF' Key in the Adjust Layer
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * | ESC  |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  |  `   |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Tab  |   Q  |   e  |   w  |   R  |   T  |                    |   Y  |   U  |   I  |   O  |   P  |  -   |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |   G  |LShift|   a  |   s  |   d  |	  f  |-------.    ,-------|   H  |   J  |   K  |   L  |   ;  |  '   |
 * |------+------+------+------+------+------| Space |    | Space |------+------+------+------+------+------|
 * |   Z  |LCTRL |   X  |   C  |   V  |   B  |-------|    |-------|   N  |   M  |   ,  |   .  |   /  |RShift|
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *                   | LAlt |SPACE |SPACE | /Space  /       \Enter \  |RAISE |BackSP| RGUI |
 *                   |      |      |      |/       /         \      \ |      |      |      |
 *                   `----------------------------'           '------''--------------------'
 */

// -- UNTESTED --

[_GAME_ESDF] = LAYOUT( \
KC_ESC,   KC_1,   KC_2,    KC_3,    KC_4,    KC_5,                     KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_GRV, \
KC_TAB,   KC_Q,   KC_E,    KC_W,    KC_R,    KC_T,                     KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_MINS, \
KC_G,  KC_LSFT,   KC_A,    KC_S,    KC_D,    KC_F,                     KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, \
KC_Z, KC_LCTRL,   KC_X,    KC_C,    KC_V,    KC_B, KC_SPC,     KC_SPC, KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,  KC_RSFT, \
                          KC_LALT, KC_SPC, KC_SPC, KC_SPC,     KC_ENT, TOG_ESDF,KC_BSPC, KC_RGUI \
)


};


void keyboard_post_init_user(void) {
  
  #ifdef CONSOLE_ENABLE
  debug_enable=true;
  #endif

  #ifdef RGBLIGHT_ENABLE
    rgblight_enable_noeeprom(); // Enables RGB, without saving settings
    rgblight_sethsv_noeeprom(HSV_PURPLE);
    rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
  #endif

}


void matrix_scan_user(void) {

    process_leader_key_user();
    hid_send_activity_ping();

}

// -- HID Code --

#ifdef RGBLIGHT_ENABLE
void set_rgblight_current_fronter(uint8_t fronter){
    switch (fronter) {
        case MEM_SWITCHED_OUT:
            rgblight_sethsv_noeeprom(HSV_WHITE);
            break;
        case MEM_FLUTTERSHY:
            rgblight_sethsv_noeeprom(HSV_YELLOW);
            break;
        case MEM_HIBIKI:
            rgblight_sethsv_noeeprom(HSV_PURPLE);
            break;
        case MEM_LUNA:
            // rgblight_sethsv_noeeprom(HSV_BLUE);
            rgblight_sethsv_noeeprom(240, 255, 200);  // Pink
            break;
        default:
            rgblight_sethsv_noeeprom(HSV_RED);
            break;
    }
}

// https://github.com/qmk/qmk_firmware/blob/master/quantum/rgblight.h
// Sets each LED to the corresponding HSV value sent from the PC
void set_rgblight_from_pc_cmd(uint8_t *led_data, uint8_t length){  // 128 Bytes.
    /* 
    * Data Format:
    * Byte n+0: LED Number
    * Byte n+1: Hue
    * Byte n+2: Saturation
    * Byte n+3: Value
    * */
    if (!rgblight_is_enabled()) {
         return;
    }
    uint8_t i;
    for (i=0; i < length; i+=4)
    {
        sethsv_split(led_data[i+1], led_data[i+2], led_data[i+3], (LED_TYPE *) &led[led_data[i]]);  // Hue, Sat, Val, LED Num
    }
    // rgblight_set();
    rgblight_set_split();

}
#endif


void hid_send_activity_ping(void){

    if (!send_activity_ping || !hid_connected || timer_elapsed(activity_ping_timer) < 5000 || timer_elapsed(last_hid_transmition_time) < 100) {
        // Only send a ping once every 5 seconds and only if we are connected.
        return;
    }
    send_activity_ping = false;
    activity_ping_timer = timer_read();  // Reset the activity ping timer
    uint8_t send_data[32] = {0};  // data packet must be 32 bytes on 8bit AVR platform
    send_data[0] = CMD_PC_ACTIVITY_PING;
    raw_hid_send(send_data, sizeof(send_data));
}


void raw_hid_send_command(uint8_t command_id, uint8_t *data, uint8_t length) {
/*
* Sent HID Data Packet Format:
* Byte 0: The Command Type
* Byte 1: Length of Command Data
* Byte 2-31: Command Data 
*
*/
    last_hid_transmition_time = timer_read();  // Reset the hid transmition timer
    uint8_t send_data[32] = {0};  // data packet must be 32 bytes on 8bit AVR platform
    send_data[0] = command_id;
    send_data[1] = length;
    memcpy(&send_data[2], data, length);
    raw_hid_send(send_data, sizeof(send_data));

    // // wait for just a short bit to avoid loosing a hid packet.
    // wait_ms(5);
}


void raw_hid_receive(uint8_t *data, uint8_t length) {
/*
* Received HID Data Packet Format:
* Byte 0: The Command Type
* Byte 1: Length of Command Data
* Byte 2-31: Command Data 
*
* NOTE: The first byte sent from the PC (Report ID) does not equal the first byte here. The Report ID is not included here.
*/
    #ifdef CONSOLE_ENABLE
    uprintf("Data Recived: len: %u, Data: [%u, %u, %u, %u, %u, %u, %u, %u, ..., %u, %u]\n", length, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], data[length-2], data[length-1]);
    #endif

    hid_connected = true;       // PC connected
    hid_disconection_timer = timer_read();  // Reset the timeout timer

    if (length >= 3) {
        uint8_t *command_id   = &(data[0]);
        uint8_t *data_length  = &(data[1]); 
        uint8_t *command_data = &(data[2]);

        switch(*command_id){
            case CMD_DO_NOTHING:  // Exists so we can keep the disconection timer alive.
                break;
            case CMD_KB_SET_CURRENT_FRONTER:
                if(current_fronter != command_data[0]){
                    current_fronter = command_data[0];
                    #ifdef RGBLIGHT_ENABLE
                    set_rgblight_current_fronter(current_fronter);
                    #endif
                }
                break;

            case CMD_KB_SET_RGB_LEDS:
                #ifdef RGBLIGHT_ENABLE
                if(*data_length % 4 == 0){  // Data Len must be a multiple of 4 for this function.
                    set_rgblight_from_pc_cmd(command_data, *data_length);
                }else{
                    ; // TODO: Handle This error case
                }
                #endif
                break;

            default:
                // We either recived a not yet supported HID call or a malformed packet. Do nothing.
                break;
        }
    }
    // raw_hid_send_response();
}

// All hid Timer code clocks in at approx 47 bytes
bool check_hid_timeout(void){

  if (!hid_connected || timer_elapsed(hid_disconection_timer) > 30000) {
    hid_connected = false;
    return false;
  }
  return true;
}

void render_current_fronter(void){
  if (check_hid_timeout()){
    switch (current_fronter) {
      case MEM_SWITCHED_OUT:
        oled_write_P(PSTR("Sleep"), false);
        break;
      case MEM_FLUTTERSHY:
        oled_write_P(PSTR("FlShy"), false);
        break;
      case MEM_HIBIKI:
        oled_write_P(PSTR("Hibik"), false);
        break;
      case MEM_LUNA:
        oled_write_P(PSTR("Luna "), false);
        break;
      case 255:
        oled_write_P(PSTR("Error"), false);
        break;
    }
  }else{
    oled_write_P(PSTR("Discn"), false);
  }
}


// --------------

// // Setting ADJUST layer RGB back to default
// void update_tri_layer_RGB(uint8_t layer1, uint8_t layer2, uint8_t layer3) {
//   if (IS_LAYER_ON(layer1) && IS_LAYER_ON(layer2)) {
//     layer_on(layer3);
//   } else {
//     layer_off(layer3);
//   }
// }


layer_state_t layer_state_set_user(layer_state_t state) {

    // Controls which layers auto shift is enabled on.
    #ifdef AUTO_SHIFT_ENABLE
    switch (biton32(state)) {
        case _QWERTY:
            autoshift_enable();
            break;
        default:
            autoshift_disable();
            break;
    }
    #endif

    // Activate ADJUST layer when LOWER & UPPER are active
    state = update_tri_layer_state(state, _RAISE, _LOWER, _ADJUST);

    // Send notification MSG to connected PC of layer state
    raw_hid_send_command(CMD_PC_NOTIFY_LAYER_CHANGE, (uint8_t *)&state, sizeof(state));
    return state;
}

//SSD1306 OLED update loop, make sure to enable OLED_DRIVER_ENABLE=yes in rules.mk
#ifdef OLED_DRIVER_ENABLE


oled_rotation_t oled_init_user(oled_rotation_t rotation) {
  if (!is_keyboard_master())
    return OLED_ROTATION_180;  // flips the display 180 degrees if offhand
  return OLED_ROTATION_270;//rotation;
}

// -- Function Definitions -- //
// When you add source files to SRC in rules.mk, you can use functions.

// amadea_oled.c
void render_default_layer_state(void);
void render_mod_status(uint8_t modifiers);

// amadea_oled_art.c
void render_moon_logo(void);

// /lib/*
// const char *read_layer_state(void);
const char *read_logo(void);
// void set_keylog(uint16_t keycode, keyrecord_t *record);
// const char *read_keylog(void);
// const char *read_keylogs(void);

// const char *read_mode_icon(bool swap);
// const char *read_host_led_state(void);
// void set_timelog(void);
// const char *read_timelog(void);


void oled_task_user(void) {
   oled_set_brightness(32);

  // OLED is 5 char wide, 16 lines long
  if (is_keyboard_master()) {
    // If you want to change the display of OLED, you need to change here

    // Show modifier status
    render_mod_status(get_mods()|get_oneshot_mods());
    
    // Show the current Fronter
    render_current_fronter();

    oled_set_cursor(0,4);  
    render_moon_logo();  //4 lines long
    oled_set_cursor(0,10);

    // render_blank_lines(10);

    // Add a empty line
    // oled_write_ln_P(PSTR("\n-----"), false);

    // Show keyboard layout
    render_default_layer_state();

    // oled_write_ln(read_timelog(), false);

  } else {
    oled_write(read_logo(), false);  // 98 bytes
  }
}

#endif // OLED_DRIVER_ENABLE

bool process_record_user(uint16_t keycode, keyrecord_t *record) {

    // hid_send_activity_ping();
    send_activity_ping = true;
    
    // -- OLED Code -- //
    #ifdef OLED_DRIVER_ENABLE
    // set_keylog(keycode, record);
    #endif
    // set_timelog();

    // - Press Down Only Macros -
    if (record->event.pressed) {
        switch (keycode) {
            // -- Macros --
            uint8_t new_fronter;
            case VRSN:  // Prints out QMK Version Info
                SEND_STRING(QMK_KEYBOARD "/" QMK_KEYMAP " @ " QMK_VERSION "\nCompiled On " QMK_BUILDDATE "\n");
                // return false;
                break;
            case SW_HIBIKI:
                new_fronter = MEM_HIBIKI;
                raw_hid_send_command(CMD_PC_SWITCH_FRONTER, &new_fronter, 1);
                // return false;
                break;
            case SW_LUNA:
                new_fronter = MEM_LUNA;
                raw_hid_send_command(CMD_PC_SWITCH_FRONTER, &new_fronter, 1);
                // return false;
                break;
            case CK_TEST:
                SEND_STRING("TEST!");
                // return false;
                break;
            case CK_SW_TEST:
                SEND_STRING("TEST!");
                break;
        }
    }

    // // - Macros With Press Down and Press up Actions-
    // switch (keycode) {
    //     case KC_:
    //         if (record->event.pressed){
                
    //         }else{

    //         }
    // }


    return true;
}