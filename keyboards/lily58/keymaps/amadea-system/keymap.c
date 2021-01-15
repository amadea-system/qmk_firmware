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
 */

#include QMK_KEYBOARD_H

#ifdef PROTOCOL_LUFA
  #include "lufa.h"
  #include "split_util.h"
#endif
#ifdef SSD1306OLED
  #include "ssd1306.h"
#endif

#include "amadea_keymap_enums.h"
#include "version.h"  // For the Version Macro

// #include "leader.c" // Include leader key definitions

// #include "oled/layer_state_reader.c" // Not needed anymore. Replaced by render_default_layer_state(void) in keymap.c


extern uint8_t is_master;

#ifdef UNICODEMAP_ENABLE
  // --- Unicode Map --- //
  enum unicode_names {
      MOON,
      BUTTERFLY,
      SMILE_CAT
  };

  const uint32_t PROGMEM unicode_map[] = {
      [MOON] = 0x1F319,      //🌙
      [BUTTERFLY] = 0x1F98B, //🦋
      [SMILE_CAT] = 0x1F63A  //😺
  };


  #define E_MOON X(MOON)
  #define E_BFLY X(BUTTERFLY)
  #define E_S_CAT X(SMILE_CAT)
#else
  // #ifdef UNICODE_ENABLE
    #define E_MOON XXXXXXX
    #define E_BFLY XXXXXXX
    #define E_S_CAT XXXXXXX
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
 * |------+------+------+------+------+------| Leader|    |    ]  |------+------+------+------+------+------|
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
 [_QWERTY] = LAYOUT( \
  KC_ESC,   KC_1,   KC_2,    KC_3,    KC_4,    KC_5,                        KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_GRV, \
  KC_TAB,   KC_Q,   KC_W,    KC_E,    KC_R,    KC_T,                        KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_MINS, \
  OSM_LSFT, KC_A,   KC_S,    KC_D,    KC_F,    KC_G,                        KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, \
  KC_LCTRL, KC_Z,   KC_X,    KC_C,    KC_V,    KC_B,  KC_LEAD,    KC_RBRC,  KC_N,    KC_M,    TD(TD_COMMA), TD(TD_DOT),  KC_SLSH,  KC_RSFT, \
                        KC_LALT, OSM_LSFT, MO(_LOWER), KC_SPC,    KC_ENT, MO(_RAISE), KC_BSPC, KC_RGUI \
),

/* LOWER - Acessed by pressing 'Lower' Key
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |  F12 |  F1  |  F2  |  F3  |  F4  |  F5  |                    |  F6  |  F7  |  F8  |  F9  | F10  | F11  |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |      |Pg Up |  Up  |Pg Dwn|   {  |                    |      |   }  |      |      |      |   =  |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |   !  | Left | Down |Right |   %  |-------.    ,-------|   ^  |   &  |   *  |      |   |  |      |
 * |------+------+------+------+------+------|   [   |    |    ]  |------+------+------+------+------+------|
 * |      |      |      |      |      |      |-------|    |-------|      |   _  |   [  |   ]  |   \  |RShift|
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
  KC_F12,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                     KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  \
  KC_NO,   KC_NO,   KC_PGUP, KC_UP,   KC_PGDN, KC_LCBR,                   KC_NO,   KC_RCBR, KC_NO,   KC_NO,   KC_NO,   KC_EQL,  \
  _______, KC_EXLM, KC_LEFT, KC_DOWN, KC_RGHT, KC_PERC,                   KC_CIRC, KC_AMPR, KC_ASTR, KC_NO,   KC_PIPE, KC_NO,   \
  _______, _______, _______, _______, _______, _______, KC_RBRC, _______, XXXXXXX, KC_UNDS, KC_LBRC, KC_RBRC, KC_BSLS, _______, \
                             _______, _______, _______, _______, _______,  _______, _______, _______\
),

// [_LOWER] = LAYOUT( 
//       ,   ,  ,  ,  ,   ,            
//       KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,     KC_TRNS, KC_TRNS,     KC_NO, KC_UNDS, KC_LBRC, KC_RBRC, KC_BSLS, KC_TRNS, 
//       KC_TRNS, KC_TRNS, KC_TRNS,                                KC_TRNS, KC_TRNS,     MO(3), KC_DEL, KC_TRNS
// ),


/* RAISE - Acessed by pressing 'Raise' Key
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |      |  🌙  |  🦋 |  😺 |      |      |                    |      |      |      |      |      |Vol Up|
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
  XXXXXXX, E_MOON,  E_BFLY,  E_S_CAT, XXXXXXX, XXXXXXX,                     XXXXXXX, XXXXXXX,       XXXXXXX,       XXXXXXX,       XXXXXXX, KC_AUDIO_VOL_UP, \
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                     XXXXXXX, XXXXXXX,       LGUI(KC_UP),   XXXXXXX,       XXXXXXX, KC_AUDIO_VOL_DOWN, \
  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,                       XXXXXXX, LGUI(KC_LEFT), LGUI(KC_DOWN), LGUI(KC_RGHT), XXXXXXX, KC_AUDIO_MUTE, \
  KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,   _______, _______,  KC_PLUS, KC_MINS,       KC_EQL,        KC_LBRC,       KC_RBRC, KC_BSLS, \
                             _______, _______, _______,  _______, _______,  _______, KC_DEL, _______ \
),

// [2] = LAYOUT(KC_NO, KC_NO, KC_NO, KC_NO,  KC_NO,  KC_NO,                      KC_NO,   KC_NO,         KC_NO,         KC_NO,         KC_NO,   KC_VOLU, 
//              KC_NO, KC_NO, KC_NO, KC_NO,  KC_NO,  KC_NO,                      KC_NO,   KC_NO,         LGUI(KC_UP),   KC_NO,         KC_NO,   KC_VOLD, 
//              KC_F1, KC_F2, KC_F3, KC_F4,  KC_F5,  KC_F6,                      KC_NO,   LGUI(KC_LEFT), LGUI(KC_DOWN), LGUI(KC_RGHT), KC_NO,   KC_MUTE, 
//              KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, KC_TRNS, KC_TRNS,   KC_PLUS, KC_MINS,       KC_EQL,        KC_LBRC,       KC_RBRC, KC_BSLS,
//                                   KC_TRNS, KC_TRNS, MO(3), KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS,       KC_TRNS
//                                   ),

/* ADJUST - Acessed by pressing 'Lower' & 'Raise' Keys
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |Versio|      |L-WASD|L-ESDF|      |      |                    |      |      |      |      |      |RESET |
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

#define TOG_ESDF TG(_GAME_ESDF)
#define TOG_WASD TG(_GAME_WASD)

[_ADJUST] = LAYOUT( \
VRSN,    XXXXXXX, TOG_WASD, TOG_ESDF, XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, RESET,   \
XXXXXXX, XXXXXXX, XXXXXXX,  XXXXXXX,  XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
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
  // Customise these values to desired behaviour
  #ifdef CONSOLE_ENABLE
  debug_enable=true;
  #endif

  #ifdef RGBLIGHT_ENABLE
    // rgblight_setrgb_range(0.5, 0.5, 0.5, 0, 5);
    rgblight_enable_noeeprom(); // Enables RGB, without saving settings
    rgblight_sethsv_noeeprom(HSV_PURPLE);
    rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
  #endif

}

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

  // -- OLED Code -- //
    #ifdef OLED_DRIVER_ENABLE
    // set_keylog(keycode, record);
    #endif
    // set_timelog();

  if (record->event.pressed) {
    switch (keycode) {

      // -- Macros --

        case VRSN:  // Prints out QMK Version Info
            SEND_STRING(QMK_KEYBOARD "/" QMK_KEYMAP " @ " QMK_VERSION "\nCompiled On " QMK_BUILDDATE "\n");
            return false;
    }
  }
  return true;
}