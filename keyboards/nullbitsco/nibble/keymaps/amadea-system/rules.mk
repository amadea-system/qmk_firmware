#   ------------   
# Nibble 65% Rules.mk
# 2020 Amadea System
#
#   ------------   

# --- Bootloader Selection --- #
BOOTLOADER = atmel-dfu      # Using Elite-C


# --- Feature Toggles --- #
OLED_DRIVER_ENABLE = yes    # OLED display
RAW_ENABLE = yes			# Enable RAw HID Communications
LEADER_ENABLE = yes         # Enable Leader Key

# --- Firmware Space Saving --- #
GRAVE_ESC_ENABLE = no
SPACE_CADET_ENABLE = no
KEY_LOCK_ENABLE = no

# --- .C Includes --- #
# Code for Leader Keys, Macros, Tap Dance, Etc
SRC +=  oled_display.c \
#		leader.c #\
#		macros.c