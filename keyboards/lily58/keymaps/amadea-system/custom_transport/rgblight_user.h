#pragma once

#include "rgblight.h"
#include "color.h"

#define RGBLIGHT_SPLIT_PIXEL_CHANGE (1 << 5)

void sethsv_split(uint8_t hue, uint8_t sat, uint8_t val, LED_TYPE *led1);
void setrgb_split(uint8_t r, uint8_t g, uint8_t b, LED_TYPE *led1);
void rgblight_set(void);
void rgblight_get_led_pixels_syncinfo(LED_TYPE *sync_leds);
void rgblight_update_sync_pixels(rgblight_syncinfo_t *syncinfo, LED_TYPE *sync_leds, bool write_to_eeprom);