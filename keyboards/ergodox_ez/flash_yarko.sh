#!/bin/bash

make ergodox_ez:yarko
echo "Flashing new firmware. Press the reset button..."
teensy_loader_cli -mmcu=atmega32u4 -w ergodox_ez_yarko.hex
