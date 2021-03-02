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

/* USB Device descriptor parameter */
#undef VENDOR_ID   
#undef PRODUCT_ID  
#undef DEVICE_VER  
#undef MANUFACTURER
#undef PRODUCT     

#define VENDOR_ID           0xFEED
#define PRODUCT_ID          0xCAFE
#define DEVICE_VER          0x0002
#define MANUFACTURER        Amadea
#define PRODUCT             QMK_Test_Platform

/* COL2ROW, ROW2COL*/
// #define DIODE_DIRECTION COL2ROW

/* key matrix size */

#undef MATRIX_ROWS
#undef MATRIX_COLS

#define MATRIX_ROWS 4
#define MATRIX_COLS 4  // 3
#define MATRIX_MASKED 

/* Keyboard Matrix Assignments */
#undef MATRIX_ROW_PINS
#undef MATRIX_COL_PINS
#define MATRIX_ROW_PINS  { B6, B2, B3, B4 }
#define MATRIX_COL_PINS  { D4, E6, B1, C6 }  // Encoders: C6  // { D4, E6, B1 }


/* Encoder Configuration */
#define ENCODERS_PAD_A { D0 }
#define ENCODERS_PAD_B { D1 }


#define LAYOUT_W_ENC( \
    k0, k1, k2, k10,\
    k3, k4, k5, k11,\
                \
        k6,     k12,\
    k7, k8, k9, k13 \
) \
{ \
    {k0,    k1, k2,    k10}, \
    {k3,    k4, k5,    k11}, \
    {KC_NO, k6, KC_NO, k12}, \
    {k7,    k8, k9,    k13}, \
}
