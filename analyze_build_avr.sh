#!/bin/bash

echo "   text    data     bss     dec     hex filename"
find .pio/build/app_uno* -name "*.o" -type f -exec avr-size -d {} \; | grep -v "text"