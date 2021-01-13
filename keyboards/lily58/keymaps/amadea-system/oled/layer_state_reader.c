/*
* This code is not being used. 
* It was replaced by void render_default_layer_state(void) in keymap.c
*/


#include QMK_KEYBOARD_H
#include <stdio.h>
#include "lily58.h"

#define L_OLED_PORTRAIT 1

#define L_BASE 0
#define L_LOWER (1 << 1)
#define L_RAISE (1 << 2)
#define L_ADJUST (1 << 3)
#define L_GAME (1 << 4)
#define L_ADJUST_TRI (L_ADJUST | L_RAISE | L_LOWER)

char layer_state_str[24];


#ifdef L_OLED_PORTRAIT
  const char *read_layer_state(void) {
    switch (layer_state)
    {
    case L_BASE:
      snprintf(layer_state_str, sizeof(layer_state_str), "Layer\nQwert");
      break;
    case L_RAISE:
      snprintf(layer_state_str, sizeof(layer_state_str), "Layer\nRaise");
      break;
    case L_LOWER: 
      snprintf(layer_state_str, sizeof(layer_state_str), "Layer\nLower");
      break;
    case L_ADJUST:
    case L_ADJUST_TRI:
      snprintf(layer_state_str, sizeof(layer_state_str), "Layer\nMod  ");
      break;
    case L_GAME:
      snprintf(layer_state_str, sizeof(layer_state_str), "Layer\nGame\nWASD ");
      break;
    default:
      snprintf(layer_state_str, sizeof(layer_state_str), "Layer\nUndef-%ld", layer_state);
    }

    return layer_state_str;
  }
#else
  const char *read_layer_state(void) {
    switch (layer_state)
    {
    case L_BASE:
      snprintf(layer_state_str, sizeof(layer_state_str), "Layer: Qwerty");
      break;
    case L_RAISE:
      snprintf(layer_state_str, sizeof(layer_state_str), "Layer: Raise");
      break;
    case L_LOWER:
      snprintf(layer_state_str, sizeof(layer_state_str), "Layer: Lower");
      break;
    case L_ADJUST:
      snprintf(layer_state_str, sizeof(layer_state_str), "Layer: Adjust");
      break;
    case L_ADJUST_TRI:
      snprintf(layer_state_str, sizeof(layer_state_str), "Layer: Adjust Tri");
      break;
    case L_GAME:
      snprintf(layer_state_str, sizeof(layer_state_str), "Layer: Game-WASD");
      break;
    default:
      snprintf(layer_state_str, sizeof(layer_state_str), "Layer: Undef-%ld", layer_state);
    }

    return layer_state_str;
  }
#endif