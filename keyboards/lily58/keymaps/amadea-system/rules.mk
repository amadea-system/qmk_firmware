# Bootloader selection
#   Teensy       halfkay
#   Pro Micro    caterina
#   Atmel DFU    atmel-dfu
#   LUFA DFU     lufa-dfu
#   QMK DFU      qmk-dfu
#   ATmega32A    bootloadHID
#   ATmega328P   USBasp
BOOTLOADER = atmel-dfu


# Build Options
#   change to "no" to disable the options, or define them in the Makefile in
#   the appropriate keymap folder that will get included automatically
#
BOOTMAGIC_ENABLE = no       # Virtual DIP switch configuration
MOUSEKEY_ENABLE = no        # Mouse keys
CONSOLE_ENABLE = yes         # Console for debug
COMMAND_ENABLE = no         # Commands for debug and configuration
BACKLIGHT_ENABLE = no       # Enable keyboard backlight functionality
MIDI_ENABLE = no            # MIDI controls
AUDIO_ENABLE = no           # Audio output on port C6
BLUETOOTH_ENABLE = no       # Enable Bluetooth with the Adafruit EZ-Key HID
RGBLIGHT_ENABLE = no       # Enable WS2812 RGB underlight. 
SWAP_HANDS_ENABLE = no      # Enable one-hand typing
TERMINAL_ENABLE = no        # Enable interactive terminal. Takes up a lot of space!
AUTO_SHIFT_ENABLE = no

NKRO_ENABLE = yes           # Nkey Rollover - if this doesn't work, see here: https://github.com/tmk/tmk_keyboard/wiki/FAQ#nkro-doesnt-work
EXTRAKEY_ENABLE = yes       # Audio control and System control
OLED_DRIVER_ENABLE = yes    # OLED display
LEADER_ENABLE = yes         # Enable Leader Key
# HAPTIC_ENABLE += DRV2605L

COMBO_ENABLE = no           # Used to allow chording of keys to trigger an action
TAP_DANCE_ENABLE = yes      # Used to allow multiple taps of a key to perform different actions
RAW_ENABLE = yes            # 204 Bytes

# We could save a few bytes by either falling back on UNICODE_ENABLE and using send_unicode_string()
UNICODE_ENABLE = no         # Basic Unicode Support
UNICODEMAP_ENABLE = yes     # Enable Unicode Map Support. <930ish Bytes>      https://docs.qmk.fm/#/feature_unicode?id=unicode-map  

LTO_ENABLE = yes            # Reduces the compiled firmware size
# EXTRAFLAGS += -flto       # https://thomasbaart.nl/2018/12/01/reducing-firmware-size-in-qmk/
GRAVE_ESC_ENABLE = no
SPACE_CADET_ENABLE = no
KEY_LOCK_ENABLE = no


# Do not enable SLEEP_LED_ENABLE. it uses the same timer as BACKLIGHT_ENABLE
SLEEP_LED_ENABLE = no       # Breathing sleep LED during USB suspend


# Code for Leader Keys, Macros, Tap Dance, ETC
SRC +=  leader.c \
        tap_dance.c \
        oled/amadea_oled.c \
        oled/amadea_oled_art.c


# If you want to change the display of OLED, you need to change here
SRC +=  ./lib/logo_reader.c \
        # ./lib/rgb_state_reader.c \
        # ./lib/keylogger.c \
        # ./lib/timelogger.c \
        # ./lib/mode_icon_reader.c \
        # ./lib/host_led_state_reader.c \
        # ./keymaps/lcd/layer_state_reader.c