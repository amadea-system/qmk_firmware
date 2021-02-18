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

#include "nibble65_keymap.h"

#include "version.h"  // For the Version Macro
#include "raw_hid.h"
#include "macros.c"

#ifdef LEADER_ENABLE
#include "leader.c"
#endif



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

// OLED Vars
uint32_t last_led_changed_time = 0;

/* ----------- Function Defs --------- */

// - HID Funcs - 
void send_hid_debug(uint8_t *data);
void raw_hid_send_command(uint8_t command_id, uint8_t *data, uint8_t length);
bool check_hid_timeout(void);
void hid_send_activity_ping(void);

/* ------ External Function Def ------ */

// -- oled_display.c --
void render_top_header(void);
void render_layer_state(void);
void render_current_fronter(void);
void render_layer_and_fronter(void);
void render_rgb_state(void);


// -- amadea_hid.c --



/* -------------- KeyMaps ------------ */

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
 * |  F15 ||   Ctrl    |  Z  |  X  |  C  |  V  |  B  |  N  |  M  |  ,  |  .  |  /  |   R-Shift   | Up  |Pg Dn |
 * |------||--------------------------------------------------------------------------------------------------|
 * | XXXX ||Lower|  Gui  |  Alt  |    Space     | Leader |    Enter   |  Raise  |    GUI   | Lft | Dwn | Rht  |
 * `------'`--------------------------------------------------------------------------------------------------'
 */
  
  #define MO_LOWER MO(_LOWER)
  #define MO_RAISE MO(_RAISE)
  #define HYPER_F19 KC_F19
  
  #ifndef LEADER_ENABLE
  #define KC_LEAD XXXXXXX
  #endif


  [_QWERTY] = LAYOUT_ansi_split_space(
               KC_ESC,   KC_1,    KC_2,    KC_3,    KC_4,    KC_5,      KC_6,    KC_7,  KC_8,    KC_9,     KC_0,    KC_MINS, KC_EQL,  KC_BSLS, KC_GRV,
    KC_MUTE,   KC_TAB,   KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,      KC_Y,    KC_U,  KC_I,    KC_O,     KC_P,    KC_LBRC, KC_RBRC, KC_BSPC, KC_DEL,
    CK_PY_LUNA,KC_LSFT,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,      KC_H,    KC_J,  KC_K,    KC_L,     KC_SCLN, KC_QUOT,          KC_ENT,  KC_PGUP,
    KC_F15,    KC_LCTL,  KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,      KC_N,    KC_M,  KC_COMM, KC_DOT,   KC_SLSH, KC_RSFT,          KC_UP,   KC_PGDN,
    HYPER_F19, KC_LALT, KC_LGUI,  MO_LOWER,         KC_SPC,  KC_LEAD,   KC_ENT,                  MO_RAISE, KC_RGUI,          KC_LEFT, KC_DOWN, KC_RGHT
  ),


/* Game-WASD - Default Layer 2  
 *         ,--------------------------------------------------------------------------------------------------.
 *         | Esc   |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  |  0  |  -  |  =  |     ~     |  \   |
 * ,------.|--------------------------------------------------------------------------------------------------|
 * | F13  || Tab    |  Q  |  W  |  E  |  R  |  T  |  Y  |  U  |  I  |  O  |  P  |  [  |  ]  |  Bkspc   | Del  |
 * |------||--------------------------------------------------------------------------------------------------|
 * | F14  || Ctrl    |  A  |  S  |  D  |  F  |  G  |  H  |  J  |  K  |  L  |  ;  |  '  |    Enter      |Pg Up |
 * |------||--------------------------------------------------------------------------------------------------|
 * | F15  ||  L-Shift  |  Z  |  X  |  C  |  V  |  B  |  N  |  M  |  ,  |  .  |  /  |   R-Shift   | Up  |Pg Dn |
 * |------||--------------------------------------------------------------------------------------------------|
 * | F16  || Ctrl |  Gui  |  Alt  |    Space     | Lower |    Enter   |  Raise  |    GUI   | Lft | Dwn | Rht  |
 * `------'`--------------------------------------------------------------------------------------------------'
 */


[_GAME_WASD] = LAYOUT_ansi_split_space(
              KC_ESC,   KC_1,    KC_2,    KC_3,    KC_4,    KC_5,      KC_6,    KC_7,  KC_8,    KC_9,     KC_0,    KC_MINS, KC_EQL,  KC_BSLS, KC_GRV,
    KC_F13,   KC_TAB,   KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,      KC_Y,    KC_U,  KC_I,    KC_O,     KC_P,    KC_LBRC, KC_RBRC, KC_BSPC, KC_DEL,
    KC_F14,   KC_LCTL,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,      KC_H,    KC_J,  KC_K,    KC_L,     KC_SCLN, KC_QUOT,          KC_ENT,  KC_PGUP,
    KC_F15,   KC_LSFT,  KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,      KC_N,    KC_M,  KC_COMM, KC_DOT,   KC_SLSH, KC_RSFT,          KC_UP,   KC_PGDN,
    KC_F16,   KC_LCTL,  KC_LGUI, KC_LALT,          KC_SPC,  MO_LOWER,  KC_ENT,                  MO_RAISE, KC_RGUI,          KC_LEFT, KC_DOWN, KC_RGHT
  ),


/* Game-MCraft - Default Layer 2  
 *         ,--------------------------------------------------------------------------------------------------.
 *         | Esc   |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  |  0  |  -  |  =  |     ~     |  \   |
 * ,------.|--------------------------------------------------------------------------------------------------|
 * | F13  || Tab    |  Q  |  W  |  E  |  R  |  T  |  Y  |  U  |  I  |  O  |  P  |  [  |  ]  |  Bkspc   | Del  |
 * |------||--------------------------------------------------------------------------------------------------|
 * | F14  || Ctrl    |  A  |  S  |  D  |  F  |  G  |  H  |  J  |  K  |  L  |  ;  |  '  |    Enter      |Pg Up |
 * |------||--------------------------------------------------------------------------------------------------|
 * | F15  ||  L-Shift  |  Z  |  X  |  C  |  V  |  B  |  N  |  M  |  ,  |  .  |  /  |   R-Shift   | Up  |Pg Dn |
 * |------||--------------------------------------------------------------------------------------------------|
 * | F16  ||Leader|  Gui  |  Alt  |    Space     | Lower |    Enter   |  Raise  |    GUI   | Lft | Dwn | Rht  |
 * `------'`--------------------------------------------------------------------------------------------------'
 */

#define MC_PROXY CK_MINECRAFT_AUTOPROXY

[_GAME_MCRAFT] = LAYOUT_ansi_split_space(
              KC_ESC,   KC_1,    KC_2,    KC_3,    KC_4,    KC_5,      KC_6,    KC_7,  KC_8,    KC_9,     KC_0,    KC_MINS, KC_EQL,  KC_BSLS, KC_GRV,
    KC_F13,   KC_TAB,   KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,      KC_Y,    KC_U,  KC_I,    KC_O,     KC_P,    KC_LBRC, KC_RBRC, KC_BSPC, KC_DEL,
    MC_PROXY, KC_LSFT,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,      KC_H,    KC_J,  KC_K,    KC_L,     KC_SCLN, KC_QUOT,          KC_ENT,  KC_PGUP,
    KC_F15,   KC_LCTL,  KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,      KC_N,    KC_M,  KC_COMM, KC_DOT,   KC_SLSH, KC_RSFT,          KC_UP,   KC_PGDN,
    KC_F16,   KC_LEAD,  KC_LGUI, KC_LALT,          KC_SPC,  MO_LOWER,  KC_ENT,                  MO_RAISE, KC_RGUI,          KC_LEFT, KC_DOWN, KC_RGHT
  ),


/* Lower - Primary alternitive layer.
 * Accessed by pressing 'Lower'
 *         ,--------------------------------------------------------------------------------------------------.
 *         | -Esc  | F1  | F2  | F3  | F4  | F5  | F6  | F7  | F8  | F9  | F10 | F11 | F12 |   - ~     | -\   |
 * ,------.|--------------------------------------------------------------------------------------------------|
 * |RGB TG|| Tab    | -Q  | Up  | -E  | -R  | -T  | -Y  | -U  | -I  | -O  | -P  | -[  | -]  |  Delete  | Del  |
 * |------||--------------------------------------------------------------------------------------------------|
 * |Prox L|| L-Shift | Lft | Dwn | Rht | -F  | -G  | -H  | -J  | -K  | -L  | -;  | -'  |    Enter      | Home |
 * |------||--------------------------------------------------------------------------------------------------|
 * |  F15 ||   Ctrl    |  Z  |  X  |  C  |  V  |  B  |  N  |  M  |  ,  |  .  |  /  |   R-Shift   |Pg Up| End  |
 * |------||--------------------------------------------------------------------------------------------------|
 * | XXXX ||Leader|  Gui  |  Alt  |    Space     | Lower |    Enter   |  Raise  |    GUI   | Lft |Pg Dn| Rht  |
 * `------'`--------------------------------------------------------------------------------------------------'
 */


  [_LOWER] = LAYOUT_ansi_split_space(  
              _______,  KC_F1,    KC_F2,    KC_F3,    KC_F4,      KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,   _______,  _______,
    RGB_TOG,  _______,  KC_UP,    _______,  _______,  CK_PY_LUNA, _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  KC_DEL,   _______,
    _______,  KC_LEFT,  KC_DOWN,  KC_RIGHT, _______,  CK_PY_FSHY, _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,  KC_HOME,
    _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,  _______,  _______,  _______,  _______,  _______,            KC_PGUP,  KC_END ,
    _______,  _______,  _______,  _______,            _______,    _______,  _______,                      _______,  _______,            _______,  KC_PGDN,  _______
  ),



/* Raise - Mostly used as a Macro layer and for infequently used symbols.
 * Accessed by pressing 'Raise'
 *         ,--------------------------------------------------------------------------------------------------.
 *         | Reset | F1  | F2  | F3  | F4  | F5  | F6  | F7  | F8  | F9  | F10 | F11 | F12 |     ~     |  \   |
 * ,------.|--------------------------------------------------------------------------------------------------|
 * | Mute || Tab    |QWERT|GWASD| -E  | -R  | -T  | -Y  | -U  | -I  | -O  | -P  | -[  | -]  |  Delete  | Del  |
 * |------||--------------------------------------------------------------------------------------------------|
 * |Sw Lun|| L-Shift | -A  | -S  | -D  | -F  | -G  | -H  | -J  | -K  | -L  | -;  | -'  |    Enter      | Home |
 * |------||--------------------------------------------------------------------------------------------------|
 * | xxxx ||   Ctrl    |  Z  |  X  |  C  |  V  |W BAK|Nyaa |  M  |  ,  |  .  |  /  |   R-Shift   |GUI U| End  |
 * |------||--------------------------------------------------------------------------------------------------|
 * |Sw Hib||Leader|  Gui  |  Alt  |    Space     | Lower |    Enter   |  Raise  |    GUI   |GUI L|GUI D| GUI R|
 * `------'`--------------------------------------------------------------------------------------------------'
 */
  

  #define GUI_LEFT LGUI(KC_LEFT)
  #define GUI_RIGHT LGUI(KC_RIGHT)
  #define GUI_UP LGUI(KC_UP)
  #define GUI_DOWN LGUI(KC_DOWN)

  #define TOG_QWERT CK_QWERTY //DF(_QWERTY)
  #define TOG_WASD  CK_WASD   //DF(_GAME_WASD)
  #define TOG_MINE  CK_MINE   //DF(_GAME_MCRAFT)
 

  [_RAISE] = LAYOUT_ansi_split_space(  
                  RESET,    KC_F1,      KC_F2,     KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,   _______,  CK_VRSN,
    GUI_UP,       _______,  TOG_QWERT,  TOG_WASD,  TOG_MINE, _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  KC_DEL,   _______,
    CK_SW_LUNA,   _______,  _______,    _______,   _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,  KC_HOME,
    _______,      _______,  _______,    _______,   _______,  _______,  KC_WBAK,  CK_NYAA,  CK_MAKE,  _______,  _______,  _______,  _______,            GUI_UP,   KC_END ,
    CK_SW_HIBIKI, _______,  _______,    _______,             _______,  _______,  _______,                      _______,  _______,            GUI_LEFT, GUI_DOWN, GUI_RIGHT
  ), 

};

void matrix_scan_user(void) {
      

    #ifdef LEADER_ENABLE
    process_leader_key_user();
    #endif

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

    last_led_changed_time = timer_read32(); 
}

void encoder_update_user(uint8_t index, bool clockwise) {
    if (layer_state_is(_LOWER)) {
        //change RGB settings
        encoder_change_RGB(clockwise);

    }else if (layer_state_is(_RAISE)){
        if (clockwise) {
            tap_code16(LGUI(KC_RIGHT)); // Right
        }else{
            tap_code16(LGUI(KC_LEFT));  // Left
        }
    
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
            // rgblight_sethsv_noeeprom(HSV_BLUE);
            rgblight_sethsv_noeeprom(240, 255, 255);  // Pink
            break;
        default:
            rgblight_sethsv_noeeprom(HSV_RED);
            break;
    }

    last_led_changed_time = timer_read32(); 
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

    last_led_changed_time = timer_read32(); 
}
#endif  // -RGBLIGHT_ENABLE


void send_hid_debug(uint8_t *data){
    raw_hid_send_command(CMD_PC_RAW_DEBUG_MSG, data, sizeof(data));
}

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


// bool oled_inactivity_check(void){
//     /*
//      Checks if the oled should be dimmed or turned off.
//      returns true if it should be off so we can avoid wtiting to the screen when off.
//      */
//     #if OLED_USER_TIMEOUT > 0

//     if(timer_elapsed32(oled_user_timeout) > OLED_USER_TIMEOUT) {
//         oled_off();
//         return true;
//     }else if(timer_elapsed32(oled_user_timeout) > OLED_DIMOUT){
//         // oled_write_ln_P(PSTR("Dimout!"), false);
//         oled_fade_set_brightness(1, 5);
//     }else{
//         // oled_write_ln_P(PSTR("Normal!!!"), false);
//         oled_fade_set_brightness(OLED_BRIGHTNESS, 15);
//     }
//     #endif

//     return false;
// }

void render_debug_layer_mask(void){

    // oled_write_P(PSTR("    "), false);
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
                oled_write_char('-', false);
            }
    } 

    oled_write_P(PSTR(" "), false);

    char def_layer_bin_num[32] = {0};
    itoa(default_layer_state, def_layer_bin_num,2);  // 90 bytes
    strrev(layer_bin_num);  // 6 bytes

    // 42 Bytes
    for (int8_t i = 0; i < 10; i++)
    {
            if (def_layer_bin_num[i] == '0' || def_layer_bin_num[i] == '1'){
                oled_write_char(def_layer_bin_num[i], false);
            }else{
                oled_write_char('-', false);
            }
    } 
}

void oled_task_user(void) {

    /* OLED is 21 chars long, 4 columns tall */

    // render_logo();
    // render_top_header();        // full line
    render_layer_and_fronter(); // full line

    // Blank line
    oled_write_ln_P(PSTR(""), false);
    oled_write_P(PSTR("    Amadea System    "), false);

    // render_debug_layer_mask();
    render_rgb_state();

    // oled_write_ln_P(PSTR("5"), false);
}

#endif  // -OLED_DRIVER_ENABLE