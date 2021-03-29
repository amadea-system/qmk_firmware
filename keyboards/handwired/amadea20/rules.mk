# MCU name
MCU = atmega32u4

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
AUDIO_ENABLE = no           # Audio output on port C6
RGBLIGHT_ENABLE = yes       # Enable WS2812 RGB underlight. 
BACKLIGHT_ENABLE = no       # Enable keyboard backlight functionality
BOOTMAGIC_ENABLE = lite     # Virtual DIP switch configuration
COMMAND_ENABLE = no         # Commands for debug and configuration
CONSOLE_ENABLE= no          # Console for debug
EXTRAKEY_ENABLE = yes       # Audio control and System control
MOUSEKEY_ENABLE = no        # Mouse keys
NKRO_ENABLE = no            # USB Nkey Rollover - if this doesn't work, see here: https://github.com/tmk/tmk_keyboard/wiki/FAQ#nkro-doesnt-work

RGBLIGHT_ENABLE = yes
SLEEP_LED_ENABLE = no   # Breathing sleep LED during USB suspend
