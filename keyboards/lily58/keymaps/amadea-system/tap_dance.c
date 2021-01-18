/* 
 * This file contains code for the Tap Dance feature
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


//tap dance states
enum tap_dance_state {
    SINGLE_TAP = 1,
    SINGLE_HOLD,
    DOUBLE_TAP,
    TRIPLE_TAP,
};

typedef struct {
    bool is_press_action;
    uint8_t state;
} tap;


// -- Function Defs --
uint8_t cur_dance(qk_tap_dance_state_t *state);

// // For the x tap dance. Put it here so it can be used in any keymap
// void x_finished(qk_tap_dance_state_t *state, void *user_data);
// void x_reset(qk_tap_dance_state_t *state, void *user_data);



// -- Functions --

// Determine the current tapdance state
uint8_t cur_dance(qk_tap_dance_state_t *state) {
    if (state->count == 1 && state->pressed) {
        return SINGLE_HOLD;
    }
    if (state->count == 1 && !state->pressed) {
        return SINGLE_TAP;
    }
    if (state->count == 2) {
        return DOUBLE_TAP;
    }
    if (state->count == 3) {
        return TRIPLE_TAP;
    }

    return 8;
}


// Create an instance of 'tap' for the 'x' tap dance.
static tap left_bracket_tap_state = {
    .is_press_action = true,
    .state = 0
};


void left_bracket_finished(qk_tap_dance_state_t *state, void *user_data) {
    left_bracket_tap_state.state = cur_dance(state);
    switch (left_bracket_tap_state.state) {
        // case SINGLE_TAP: register_code(KC_COMMA); break;
        // case SINGLE_HOLD: register_code(KC_COMMA); break;
        // case DOUBLE_TAP: register_code(KC_LBRACKET); break;
        // case TRIPLE_TAP: 
        //     register_code(KC_LSFT);
        //     register_code(KC_LBRACKET);
        //     break;

        case SINGLE_TAP: register_code(KC_LBRACKET); break;
        case SINGLE_HOLD: register_code(KC_LBRACKET); break;
        // case DOUBLE_TAP: register_code(KC_LBRACKET); break;
        case DOUBLE_TAP: 
            register_code(KC_LSFT);
            register_code(KC_LBRACKET);
            break;
    }
}

void left_bracket_reset(qk_tap_dance_state_t *state, void *user_data) {
    switch (left_bracket_tap_state.state) {
    
        case SINGLE_TAP: unregister_code(KC_LBRACKET); break;
        case SINGLE_HOLD: unregister_code(KC_LBRACKET); break;
        // case DOUBLE_TAP: unregister_code(KC_LBRACKET); break;
        case DOUBLE_TAP: 
            unregister_code(KC_LSFT);
            unregister_code(KC_LBRACKET);
            break;
    }
    left_bracket_tap_state.state = 0;
}


// Create an instance of 'tap' for the 'x' tap dance.
static tap right_bracket_tap_state = {
    .is_press_action = true,
    .state = 0
};


void right_bracket_finished(qk_tap_dance_state_t *state, void *user_data) {
    right_bracket_tap_state.state = cur_dance(state);
    switch (right_bracket_tap_state.state) {
        // case SINGLE_TAP: register_code(KC_DOT); break;
        // case SINGLE_HOLD: register_code(KC_DOT); break;
        // case DOUBLE_TAP: register_code(KC_RBRACKET); break;
        // case TRIPLE_TAP: 
        //     register_code(KC_LSFT);
        //     register_code(KC_RBRACKET);
        //     break;

        case SINGLE_TAP: register_code(KC_RBRACKET); break;
        case SINGLE_HOLD: register_code(KC_RBRACKET); break;
        // case DOUBLE_TAP: register_code(KC_RBRACKET); break;
        case DOUBLE_TAP: 
            register_code(KC_LSFT);
            register_code(KC_RBRACKET);
            break;
    }
}

void right_bracket_reset(qk_tap_dance_state_t *state, void *user_data) {
    switch (right_bracket_tap_state.state) {
        case SINGLE_TAP: unregister_code(KC_RBRACKET); break;
        case SINGLE_HOLD: unregister_code(KC_RBRACKET); break;
        // case DOUBLE_TAP: unregister_code(KC_RBRACKET); break;
        case DOUBLE_TAP: 
            unregister_code(KC_LSFT);
            unregister_code(KC_RBRACKET);
            break;

    }
    right_bracket_tap_state.state = 0;
}




//associate the tap dance keys with their functionality
qk_tap_dance_action_t tap_dance_actions[] = {
    [TD_LBRK] = ACTION_TAP_DANCE_FN_ADVANCED_TIME(NULL, left_bracket_finished, left_bracket_reset, 200),
    [TD_RBRK] = ACTION_TAP_DANCE_FN_ADVANCED_TIME(NULL, right_bracket_finished, right_bracket_reset, 200),
    [TD_MINUS_EQL]  = ACTION_TAP_DANCE_DOUBLE(KC_MINS, KC_EQL),

};