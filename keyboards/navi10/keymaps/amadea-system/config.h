/*
This is the c configuration file for the keymap

Copyright 2020 Amadea System

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

// Backlight Config
#define BACKLIGHT_LEVELS 16      //  The number of brightness levels (maximum 31 excluding off)
#define BACKLIGHT_LIMIT_VAL 100 //128
#define BACKLIGHT_BREATHING
#define BREATHING_PERIOD 6
#define BACKLIGHT_PIN B5
#define BACKLIGHT_ON_STATE 0

// ARGB Underlight Config
#undef RGBLED_NUM
#define RGBLED_NUM 10
#define RGB_DI_PIN F4
#define RGBLIGHT_LIMIT_VAL 150

#define RGBLIGHT_ANIMATIONS



