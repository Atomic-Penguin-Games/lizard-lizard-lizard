#!/bin/bash

# Build script for Windows cross-compilation using clang with mingw64

set -e

echo "Building Lizard Meme Game for Windows..."

# Create build directory for Windows
BUILD_DIR="build-windows"
if [ -d "$BUILD_DIR" ]; then
    echo "Cleaning existing Windows build directory..."
    rm -rf "$BUILD_DIR"
fi

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# Configure with the mingw64 toolchain
echo "Configuring CMake for Windows cross-compilation..."
cmake .. -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchain-mingw64.cmake \
         -DCMAKE_BUILD_TYPE=Release

# Build the project
echo "Building the project..."
make -j$(nproc)

echo "Windows build complete!"
echo "Executable: $BUILD_DIR/lizard_meme.exe"
echo ""
echo "To run on Windows, copy the following files:"
echo "- lizard_meme.exe"
echo "- res/ directory"
echo ""
echo "Note: Make sure the target Windows system has the required runtime libraries."
