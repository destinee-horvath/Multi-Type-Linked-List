#!/bin/bash

dir="tests/partA"
out_dir="out_tests"

if [ ! -d "$dir" ]; then
    echo "Error: Directory '$dir' not found."
    exit 1
fi

mkdir -p "$out_dir"

echo ""
echo ""
echo "TEST PART A:"

X=1
for file in "$dir"/*.in; do
    echo "-------------------- Test $X --------------------"

    filename=$(basename "$file")  #filename without path 
    out_file="$out_dir/out_${filename%.in}.out"

    touch "$out_file"

    expected_file="${file%.in}.expected"

    ./mtll < "$file" > "$out_file"

    if diff "$out_file" "$expected_file" >/dev/null; then
        echo "Passed: $filename"
    else
        echo "Failed: $filename"
        echo "-- Expected output --"
        cat "$expected_file"
        echo ""
        echo "-- Actual output --"
        cat "$out_file"
    fi

    ((X++))

done  


## TEST B ####################
dir="tests/partB"
out_dir="out_tests"

if [ ! -d "$dir" ]; then
    echo "Error: Directory '$dir' not found."
    exit 1
fi

mkdir -p "$out_dir"

echo ""
echo ""
echo "TEST PART B:"

X=1
for file in "$dir"/*.in; do
    echo "-------------------- Test $X --------------------"

    filename=$(basename "$file") 
    out_file="$out_dir/out_${filename%.in}.out"

    touch "$out_file"

    expected_file="${file%.in}.expected"

    ./mtll < "$file" > "$out_file"

    if diff "$out_file" "$expected_file" >/dev/null; then
        echo "Passed: $filename"
    else
        echo "Failed: $filename"
        echo "-- Expected output --"
        cat "$expected_file"
        echo ""
        echo "-- Actual output --"
        cat "$out_file"
    fi

    ((X++))

done  

## TEST C ####################
dir="tests/partC"
out_dir="out_tests"

if [ ! -d "$dir" ]; then
    echo "Error: Directory '$dir' not found."
    exit 1
fi

mkdir -p "$out_dir"

echo ""
echo ""
echo "TEST PART C:"

X=1
for file in "$dir"/*.in; do
    echo "-------------------- Test $X --------------------"

    filename=$(basename "$file")  
    out_file="$out_dir/out_${filename%.in}.out"

    touch "$out_file"

    expected_file="${file%.in}.expected"

    ./mtll < "$file" > "$out_file"

    if diff "$out_file" "$expected_file" >/dev/null; then
        echo "Passed: $filename"
    else
        echo "Failed: $filename"
        echo "-- Expected output --"
        cat "$expected_file"
        echo ""
        echo "-- Actual output --"
        cat "$out_file"
    fi

    ((X++))

done  

rm -rf "$out_dir"

