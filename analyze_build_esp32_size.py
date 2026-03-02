#!/usr/bin/env python3
"""
Simple script to calculate flash memory usage from .o list file.

Usage:
    ./flash_sum.py fileSymbols.txt
"""

import sys
from pathlib import Path

def main():
    if len(sys.argv) != 2:
        print("Usage: python flash_sum.py <symbols_file>")
        sys.exit(1)
    
    filename = sys.argv[1]
    if not Path(filename).exists():
        print(f"Error: File '{filename}' not found")
        sys.exit(1)
    
    flash_total = 0
    ram_total = 0
    total_objects = 0
    
    with open(filename, 'r') as f:
        for line_num, line in enumerate(f, 1):
            line = line.strip()
            if not line:
                continue
            
            parts = line.split()
            print(parts)
            if len(parts) < 3:
                continue
            
            try:
                # Parse: address size type symbol_name
                text = int(parts[0])
                data = int(parts[1])
                bss =  int(parts[2])
                
                flash_total += text + data

                ram_total += data + bss
                
                total_objects += 1
                
            except ValueError:
                # Skip lines with invalid size
                continue
    
    # Print results
    print("\n" + "=" * 60)
    print("MEMORY USAGE SUMMARY")
    print("=" * 60)
    print(f"Total objects analyzed: {total_objects:,}")
    print()
    print(f"FLASH: {flash_total:12,} bytes ({flash_total/1024:9.2f} KB, {flash_total/(1024*1024):.2f} MB)")
    print(f"RAM:   {ram_total:12,} bytes ({ram_total/1024:9.2f} KB)")
    print("=" * 60)

if __name__ == "__main__":
    main()