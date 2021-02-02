# Bootloader selection
#   Teensy       halfkay
#   Pro Micro    caterina
#   Atmel DFU    atmel-dfu
#   LUFA DFU     lufa-dfu
#   QMK DFU      qmk-dfu
#   ATmega32A    bootloadHID
#   ATmega328P   USBasp
BOOTLOADER = caterina


# Build Options
#   change to "no" to disable the options, or define them in the Makefile in
#   the appropriate keymap folder that will get included automatically
#
TAP_DANCE_ENABLE = yes
BACKLIGHT_ENABLE = yes      # Enable keyboard backlight functionality
RGBLIGHT_ENABLE = yes  		# Enable WS2812 RGB underlight. 
RAW_ENABLE = yes            # 204 Bytes
CONSOLE_ENABLE = no

OLED_DRIVER_ENABLE = yes    # OLED display

EXTRAFLAGS += -flto         # https://thomasbaart.nl/2018/12/01/reducing-firmware-size-in-qmk/
