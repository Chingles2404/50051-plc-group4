#!/bin/bash

set -e

# Remove build and Testing directories
echo "Removing build and Testing directories..."
rm -rf build Testing
echo "Cleanup done."


# detect make tool
if command -v make &>/dev/null; then
    MAKE_TOOL=make
    echo "Using make"
else
    echo "Error: 'make' not found"
    exit 1
fi

echo "Configuring CMake..."
cmake -S . -B build
if [ $? -ne 0 ]; then
    echo "Error: CMake configuration failed"
    exit 1
fi


echo "Building project..."
cmake --build build
if [ $? -ne 0 ]; then
    echo "Error: Build failed"
    exit 1
fi

# Run binary
INPUT_BMP="sample_inputs/example.bmp"
EXE_PATH="build/ImageToASCIILineArt"
OUTPUT_TXT="build/ImageOutput.txt"

if [ -f "$INPUT_BMP" ]; then
    # echo "Info: Converting image to ASCII art..."
    # echo "Info: Output will be saved to $OUTPUT_TXT"

    "$EXE_PATH"
else
    echo "Error: $INPUT_BMP not found"
    exit 1
fi
