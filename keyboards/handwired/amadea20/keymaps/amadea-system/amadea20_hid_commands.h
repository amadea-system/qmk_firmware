/* 
 * Definitions for Hid Command Protocal
 * 
 * Copyright 2021 Amadea System
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

#pragma once

#include "quantum.h"


/* --------- External Functions --------- */
// void queue_activity_ping(void);
// void send_queued_activity_ping(void);

bool check_hid_timeout(void);
void raw_hid_send_command(uint8_t command_id, uint8_t *data, uint8_t length);

// void send_hid_cmd_change_volume(uint8_t volume_direction, uint8_t target);
// void send_hid_cmd_change_volume(uint8_t volume_direction, uint8_t volume_amount, uint8_t target);
// void send_hid_cmd_get_next_volume_process(uint8_t search_direction);
// void send_queued_incremental_volume_change(void);
// void queue_volume_change(int8_t volume_amount, uint8_t volume_target);

// bool check_hid_activity_ping(void);

/* ---------------- Enums -------------- */


// HID Command Protocol Identifiers
enum hid_commands {
    CMD_DO_NOTHING = 0,  // Do Nothing

    // CMDs Sent From PC
    CMD_KB_SET_CURRENT_FRONTER = 1,
    CMD_KB_SET_ALL_RGB_LEDS,
    CMD_KB_SET_RGB_LEDS,
    CMD_KB_ACTIVITY_PING,
    CMD_KB_SHOW_CURRENT_VOLUME,

    // CMDs Sent To PC
    CMD_PC_RAW_DEBUG_MSG = 120,
    CMD_PC_DEBUG_MSG,
    CMD_PC_SWITCH_FRONTER,
    CMD_PC_NOTIFY_LAYER_CHANGE,
    CMD_PC_ACTIVITY_PING,
    CMD_PC_CHANGE_VOLUME,
    CMD_PC_GET_VOL_PROCESS
};

enum volume_change_direction {
    VOLUME_DOWN_INC = 0,
    VOLUME_UP_INC,
    VOLUME_MUTE_TOG,
    VOLUME_SET
};

enum volume_change_target {
    VOLUME_TARGET_FOCUSED = 0,
    VOLUME_TARGET_SYSTEM,
    VOLUME_TARGET_PROCESS
};

