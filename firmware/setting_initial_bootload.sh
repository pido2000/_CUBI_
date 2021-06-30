#!/bin/bash
echo "hit the reset button"
sleep 2
dfu-util -a1 -d 1eaf:0003 -D ./bootloader_default.bin 
