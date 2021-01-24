#include "rgblight_user.h"


#if defined(RGBLIGHT_ENABLE) && defined(RGBLED_SPLIT) && defined(RGBLIGHT_CUSTOM_DRIVER)

#include "ws2812.h"
#include "string.h" // For memcpy


extern rgblight_config_t rgblight_config; // Declared in rgblight.c
extern rgblight_status_t rgblight_status; // Declared in rgblight.c

#define RGBLIGHT_STATUS_CHANGE_PIXELS rgblight_status.change_flags |= RGBLIGHT_SPLIT_PIXEL_CHANGE

void sethsv_raw_u(uint8_t hue, uint8_t sat, uint8_t val, LED_TYPE *led1) {
    HSV hsv = {hue, sat, val};
    RGB rgb = hsv_to_rgb(hsv);
    setrgb_split(rgb.r, rgb.g, rgb.b, led1);
}

void sethsv_split(uint8_t hue, uint8_t sat, uint8_t val, LED_TYPE *led1) { sethsv_raw_u(hue, sat, val > RGBLIGHT_LIMIT_VAL ? RGBLIGHT_LIMIT_VAL : val, led1); }

void setrgb_split(uint8_t r, uint8_t g, uint8_t b, LED_TYPE *led1) {

    if(led1->r != r || led1->g != g || led1->b != b ){
        RGBLIGHT_STATUS_CHANGE_PIXELS;
    }
    led1->r = r;
    led1->g = g;
    led1->b = b;
#ifdef RGBW
    led1->w = 0;
#endif
}

void rgblight_get_led_pixels_syncinfo(LED_TYPE *sync_leds) {
    memcpy(sync_leds, &led, sizeof(led));
}

void rgblight_update_led_pixels_on_slave(LED_TYPE *sync_leds) {
    memcpy(&led, sync_leds, sizeof(led));
}

/* for split keyboard slave side */
void rgblight_update_sync_pixels(rgblight_syncinfo_t *syncinfo, LED_TYPE *sync_leds, bool write_to_eeprom) {
#    ifdef RGBLIGHT_LAYERS
    if (syncinfo->status.change_flags & RGBLIGHT_STATUS_CHANGE_LAYERS) {
        rgblight_status.enabled_layer_mask = syncinfo->status.enabled_layer_mask;
    }
#    endif
    if (syncinfo->status.change_flags & RGBLIGHT_STATUS_CHANGE_MODE) {
        if (syncinfo->config.enable) {
            rgblight_config.enable = 1;  // == rgblight_enable_noeeprom();
            rgblight_mode_eeprom_helper(syncinfo->config.mode, write_to_eeprom);
        } else {
            rgblight_disable_noeeprom();
        }
    }
    if (syncinfo->status.change_flags & RGBLIGHT_STATUS_CHANGE_HSVS) {
        rgblight_sethsv_eeprom_helper(syncinfo->config.hue, syncinfo->config.sat, syncinfo->config.val, write_to_eeprom);
        // rgblight_config.speed = config->speed; // NEED???
    }

    if (syncinfo->status.change_flags & RGBLIGHT_SPLIT_PIXEL_CHANGE) {
        rgblight_update_led_pixels_on_slave(sync_leds);
        rgblight_set();
    }

#    ifdef RGBLIGHT_USE_TIMER
    if (syncinfo->status.change_flags & RGBLIGHT_STATUS_CHANGE_TIMER) {
        if (syncinfo->status.timer_enabled) {
            rgblight_timer_enable();
        } else {
            rgblight_timer_disable();
        }
    }
#        ifndef RGBLIGHT_SPLIT_NO_ANIMATION_SYNC
    if (syncinfo->status.change_flags & RGBLIGHT_STATUS_ANIMATION_TICK) {
        animation_status.restart = true;
    }
#        endif /* RGBLIGHT_SPLIT_NO_ANIMATION_SYNC */
#    endif     /* RGBLIGHT_USE_TIMER */
}

void rgblight_call_driver(LED_TYPE *start_led, uint8_t num_leds) { ws2812_setleds(start_led, num_leds); }

void rgblight_set(void) {
    LED_TYPE *start_led;
    uint8_t   num_leds = rgblight_ranges.clipping_num_leds;

    if (!rgblight_config.enable) {
        for (uint8_t i = rgblight_ranges.effect_start_pos; i < rgblight_ranges.effect_end_pos; i++) {
            led[i].r = 0;
            led[i].g = 0;
            led[i].b = 0;
#    ifdef RGBW
            led[i].w = 0;
#    endif
        }
    }

#    ifdef RGBLIGHT_LAYERS
    if (rgblight_layers != NULL
#        if !defined(RGBLIGHT_LAYERS_OVERRIDE_RGB_OFF)
        && rgblight_config.enable
#        elif defined(RGBLIGHT_SLEEP)
        && !is_suspended
#        endif
    ) {
        rgblight_layers_write();
    }
#    endif

#    ifdef RGBLIGHT_LED_MAP
    LED_TYPE led0[RGBLED_NUM];
    for (uint8_t i = 0; i < RGBLED_NUM; i++) {
        led0[i] = led[pgm_read_byte(&led_map[i])];
    }
    start_led = led0 + rgblight_ranges.clipping_start_pos;
#    else
    start_led = led + rgblight_ranges.clipping_start_pos;
#    endif

#    ifdef RGBW
    for (uint8_t i = 0; i < num_leds; i++) {
        convert_rgb_to_rgbw(&start_led[i]);
    }
#    endif
    rgblight_call_driver(start_led, num_leds);
}
#endif
