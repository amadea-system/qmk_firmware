/* Copyright 2020 Jay Greco
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

#include "nibble65_keymap_enums.h"

#include "version.h"  // For the Version Macro
#include "raw_hid.h"
#include "macros.c"


/* ------------ # Defines ------------ */

// - HID -
#define HID_DISCONECTION_TIMEOUT 5000  // milliseconds 


/* --------- Local Variables --------- */

// HID Vars
uint8_t current_fronter = MEM_SWITCHED_OUT;
bool hid_connected = false; // Flag indicating if we have a PC connection yet
static uint16_t hid_disconection_timer = 0;
static uint16_t activity_ping_timer = 0;

bool send_activity_ping = false;
static uint16_t last_hid_transmition_time = 0;


/* ----------- Function Defs --------- */

// - HID Funcs - 
void send_hid_debug(uint8_t *data);
void raw_hid_send_command(uint8_t command_id, uint8_t *data, uint8_t length);
bool check_hid_timeout(void);
void hid_send_activity_ping(void);


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    // [_MA] = LAYOUT_ansi(
    //         KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC, KC_TILD,
    // KC_F13, KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS, KC_DEL,
    // KC_F14, KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,  KC_PGUP,
    // KC_F15, KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,          KC_UP,   KC_PGDN,
    // KC_F16, KC_LCTL, KC_LGUI, KC_LALT,                   KC_SPC,                    MO(_FN), KC_RALT, KC_RCTL,          KC_LEFT, KC_DOWN, KC_RGHT

/* Qwerty - Default Layer
 *         ,--------------------------------------------------------------------------------------------------.
 *         | Esc   |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  |  0  |  -  |  =  |     ~     |  \   |
 * ,------.|--------------------------------------------------------------------------------------------------|
 * | Mute || Tab    |  Q  |  W  |  E  |  R  |  T  |  Y  |  U  |  I  |  O  |  P  |  [  |  ]  |  Bkspc   | Del  |
 * |------||--------------------------------------------------------------------------------------------------|
 * |Prox L|| L-Shift |  A  |  S  |  D  |  F  |  G  |  H  |  J  |  K  |  L  |  ;  |  '  |    Enter      |Pg Up |
 * |------||--------------------------------------------------------------------------------------------------|
 * |  F15 ||   Ctrl    |  Z  |  X  |  C  |  V  |  B  |  N  |  M  |  ,  |  .  |  /  |    Raise    | Up  |Pg Dn |
 * |------||--------------------------------------------------------------------------------------------------|
 * | XXXX || XXXX |  Gui  |  Alt  |    Space     | Lower |    Enter   |  Raise  |    GUI   | Lft | Dwn | Rht  |
 * `------'`--------------------------------------------------------------------------------------------------'
 */
  
  #define MO_LOWER MO(_LOWER)
  #define MO_RAISE MO(_RAISE)

  [_QWERTY] = LAYOUT_ansi_split_space(
               KC_ESC,   KC_1,    KC_2,    KC_3,    KC_4,    KC_5,      KC_6,    KC_7,  KC_8,    KC_9,     KC_0,    KC_MINS, KC_EQL,  KC_TILD, KC_BSLS,
    KC_MUTE,   KC_TAB,   KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,      KC_Y,    KC_U,  KC_I,    KC_O,     KC_P,    KC_LBRC, KC_RBRC, KC_BSPC, KC_DEL,
    CK_PY_LUNA,KC_LSFT,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,      KC_H,    KC_J,  KC_K,    KC_L,     KC_SCLN, KC_QUOT,          KC_ENT,  KC_PGUP,
    KC_F15,    KC_LCTL,  KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,      KC_N,    KC_M,  KC_COMM, KC_DOT,   KC_SLSH, KC_RSFT,          KC_UP,   KC_PGDN,
    XXXXXXX,   XXXXXXX,  KC_LGUI, KC_LALT,          KC_SPC,  MO_LOWER,  KC_ENT,                  MO_RAISE, KC_RGUI,          KC_LEFT, KC_DOWN, KC_RGHT
  ),


/* Lower - Primary alternitive layer.
 * Accessed by pressing 'Lower'
 *         ,--------------------------------------------------------------------------------------------------.
 *         | -Esc  | F1  | F2  | F3  | F4  | F5  | F6  | F7  | F8  | F9  | F10 | F11 | F12 |   - ~     | -\   |
 * ,------.|--------------------------------------------------------------------------------------------------|
 * |RGB TG|| Tab    | -Q  | -W  | -E  | -R  | -T  | -Y  | -U  | -I  | -O  | -P  | -[  | -]  |  Delete  | Del  |
 * |------||--------------------------------------------------------------------------------------------------|
 * |Prox L|| L-Shift | -A  | -S  | -D  | -F  | -G  | -H  | -J  | -K  | -L  | -;  | -'  |    Enter      | Home |
 * |------||--------------------------------------------------------------------------------------------------|
 * |  F15 ||   Ctrl    |  Z  |  X  |  C  |  V  |  B  |  N  |  M  |  ,  |  .  |  /  |    Raise    |Pg Up| End  |
 * |------||--------------------------------------------------------------------------------------------------|
 * | XXXX || XXXX |  Gui  |  Alt  |    Space     | Lower |    Enter   |   Alt   |    GUI   | Lft |Pg Dn| Rht  |
 * `------'`--------------------------------------------------------------------------------------------------'
 */

  [_LOWER] = LAYOUT_ansi_split_space(  
              _______,  KC_F1,    KC_F2,    KC_F3,    KC_F4,      KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,   _______,  _______,
    RGB_TOG,  _______,  _______,  _______,  _______,  CK_PY_LUNA, _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  KC_DEL,   _______,
    _______,  _______,  _______,  _______,  _______,  CK_PY_FSHY, _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,  KC_HOME,
    _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,  _______,  _______,  _______,  _______,  _______,            KC_PGUP,  KC_END ,
    _______,  _______,  _______,  _______,            _______,    _______,  _______,                      _______,  _______,            _______,  KC_PGDN,  _______
  ),


/* Raise - Mostly used as a Macro layer and for infequently used symbols.
 * Accessed by pressing 'Raise'
 *         ,--------------------------------------------------------------------------------------------------.
 *         | Reset | F1  | F2  | F3  | F4  | F5  | F6  | F7  | F8  | F9  | F10 | F11 | F12 |     ~     |  \   |
 * ,------.|--------------------------------------------------------------------------------------------------|
 * | Mute || Tab    | -Q  | -W  | -E  | -R  | -T  | -Y  | -U  | -I  | -O  | -P  | -[  | -]  |  Delete  | Del  |
 * |------||--------------------------------------------------------------------------------------------------|
 * |Sw Lun|| L-Shift | -A  | -S  | -D  | -F  | -G  | -H  | -J  | -K  | -L  | -;  | -'  |    Enter      | Home |
 * |------||--------------------------------------------------------------------------------------------------|
 * |Sw Hib||   Ctrl    |  Z  |  X  |  C  |  V  |W BAK|Nyaa |  M  |  ,  |  .  |  /  |    Raise    |GUI U| End  |
 * |------||--------------------------------------------------------------------------------------------------|
 * | XXXX || XXXX |  Gui  |  Alt  |    Space     | Lower |    Enter   |   Alt   |    GUI   |GUI L|GUI D| GUI R|
 * `------'`--------------------------------------------------------------------------------------------------'
 */
  
  #define GUI_LEFT LGUI(KC_LEFT)
  #define GUI_RIGHT LGUI(KC_RIGHT)
  #define GUI_UP LGUI(KC_UP)
  #define GUI_DOWN LGUI(KC_DOWN)

  [_RAISE] = LAYOUT_ansi_split_space(  
                  RESET,    KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,   _______,  CK_VRSN,
    RGB_TOG,      _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  KC_DEL,   _______,
    CK_SW_HIBIKI, _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,  KC_HOME,
    CK_SW_LUNA,   _______,  _______,  _______,  _______,  _______,  KC_WBAK,  CK_NYAA,  _______,  _______,  _______,  _______,  _______,            GUI_UP,   KC_END ,
    _______,      _______,  _______,  _______,            _______,  _______,  _______,                      _______,  _______,            GUI_LEFT, GUI_DOWN, GUI_RIGHT
  ),

};

void matrix_scan_user(void) {

    hid_send_activity_ping();
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  // Send keystrokes to host keyboard, if connected (see readme)
  // process_record_remote_kb(keycode, record);

    send_activity_ping = true;

    bool continue_key_processing = process_macros(keycode, record);

    if (!continue_key_processing){
        return false;
    }

    return true;
}

// RGB config, for changing RGB settings on non-VIA firmwares
void encoder_change_RGB(bool clockwise) {
    bool shift = get_mods() & MOD_MASK_SHIFT;
    bool alt = get_mods() & MOD_MASK_ALT;
    bool ctrl = get_mods() & MOD_MASK_CTRL;

    if (clockwise) {
        if (alt) {
            rgblight_increase_hue();
        } else if (ctrl) {
            rgblight_increase_val();
        } else if (shift) {
            rgblight_increase_sat();
        } else {
            rgblight_step();
        }

  } else {
      if (alt) {
            rgblight_decrease_hue();
        } else if (ctrl) {
            rgblight_decrease_val();
        } else if (shift) {
            rgblight_decrease_sat();
        } else {
            rgblight_step_reverse();
        }
    } 
}

void encoder_update_kb(uint8_t index, bool clockwise) {
    if (layer_state_is(_LOWER)) {
      //change RGB settings
        encoder_change_RGB(clockwise);
    }else{
        if (clockwise) {
            tap_code(KC_VOLU);
        }else{
            tap_code(KC_VOLD);
        }  
    }
}


// -- HID Code --
#ifdef RAW_ENABLE

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
            rgblight_sethsv_noeeprom(HSV_BLUE);
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
        sethsv(led_data[i+1], led_data[i+2], led_data[i+3], (LED_TYPE *) &led[led_data[i]]);  // Hue, Sat, Val, LED Num
    }
    rgblight_set();
}
#endif  // -RGBLIGHT_ENABLE


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
}

// All hid Timer code clocks in at approx 47 bytes
bool check_hid_timeout(void){

    if (!hid_connected || timer_elapsed(hid_disconection_timer) > HID_DISCONECTION_TIMEOUT) {
        hid_connected = false;
        return false;
    }
    return true;
}

#endif // -HID RAW_ENABLE 
// --------------


#ifdef OLED_DRIVER_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) { return OLED_ROTATION_0; }



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

#ifdef RGBLIGHT_ENABLE
extern rgblight_config_t rgblight_config;
void render_rgb_state(void) {

    // char buf[30];

    // snprintf(buf, sizeof(buf), " LED %2d: %d,%d,%d ",
    //          rgblight_get_mode(),
    //          rgblight_get_hue()/RGBLIGHT_HUE_STEP,
    //          rgblight_get_sat()/RGBLIGHT_SAT_STEP,
    //          rgblight_get_val()/RGBLIGHT_VAL_STEP);
    // oled_write(buf, false);

    char rbf_info_str[24];
    snprintf(rbf_info_str, sizeof(rbf_info_str), "%s %2d h%3d s%3d v%3d",
             rgblight_config.enable ? "on" : "- ", rgblight_config.mode,
             (uint8_t)((rgblight_config.hue*100)/255), 
             (uint8_t)((rgblight_config.sat*100)/255), 
             (uint8_t)((rgblight_config.val*100)/255));
    oled_write(rbf_info_str, false);
}
#endif // RGBLIGHT_ENABLE



void oled_task_user(void) {

    /* OLED is 21 chars long, 4 columns tall */

    // render_logo();
    // render_top_header();        // full line
    render_layer_and_fronter(); // full line

    // Blank line
    oled_write_ln_P(PSTR(" "), false);
    oled_write_P(PSTR("    Amadea System    "), false);
    render_rgb_state();

    

    // oled_write_ln_P(PSTR("5"), false);
}
#endif  // -OLED_DRIVER_ENABLE