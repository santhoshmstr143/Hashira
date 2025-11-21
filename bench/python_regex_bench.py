#!/usr/bin/env python3
"""
Simple Python regex benchmark script.
Usage: python_regex_bench.py <text_file> <pattern_file>
Prints: <count> <time_ms> to stdout
"""
import sys
import re
import time

if len(sys.argv) != 3:
    print("0 0.0")
    sys.exit(0)

text_file = sys.argv[1]
pattern_file = sys.argv[2]

try:
    with open(text_file, 'r') as f:
        text = f.read().strip()
    
    with open(pattern_file, 'r') as f:
        pattern = f.read().strip()
        
    start_time = time.time()
    matches = list(re.finditer(pattern, text))
    end_time = time.time()
    
    count = len(matches)
    time_ms = (end_time - start_time) * 1000.0
    
    print(f"{count} {time_ms:.4f}")

except Exception as e:
    print("0 0.0")
