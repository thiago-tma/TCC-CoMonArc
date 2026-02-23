#!/bin/bash

echo "   text    data     bss     dec     hex filename"
find .pio/build/app_esp32_uart -name "*.o" -type f -exec ~/.platformio/packages/toolchain-xtensa-esp32/bin/xtensa-esp32-elf-size -d {} \; | grep -v "text"