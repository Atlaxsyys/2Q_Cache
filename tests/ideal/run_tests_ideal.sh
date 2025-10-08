#!/bin/bash

for i in {1..20}; do
    formatted_idx=$(printf "%03d" $i)
    
    if [[ ! -f "${formatted_idx}.dat" || ! -f "${formatted_idx}.sol" ]]; then
        echo "Test ${formatted_idx} failed: required files not found"
        continue
    fi

    actual=$(../../build/cache_ideal < "${formatted_idx}.dat")
    expected=$(cat "${formatted_idx}.sol")
    
    if [ "$actual" = "$expected" ]; then
        echo "Test ${formatted_idx} passed"
    else
        echo "Test ${formatted_idx} failed: expected '$expected', got '$actual'"
    fi
done