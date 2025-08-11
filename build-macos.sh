#!/bin/bash

# Build script for macOS cross-compilation

echo "Building for macOS..."

# Check if osxcross is installed
if [ ! -d "/opt/osxcross" ]; then
    echo "Error: osxcross not found. Please install osxcross first."
    echo "See: https://github.com/tpoechtrager/osxcross"
    exit 1
fi

# Check if macOS raylib exists
if [ ! -d "../raylib-macos" ]; then
    echo "Error: raylib-macos not found. Please build raylib for macOS first."
    echo "You need to compile raylib using osxcross and place it in ../raylib-macos/"
    exit 1
fi

# Create build directory
mkdir -p build-macos
cd build-macos

# Configure with macOS toolchain
cmake .. \
    -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchain-macos.cmake \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_OSX_DEPLOYMENT_TARGET=10.15

# Build
make -j$(nproc)

if [ $? -eq 0 ]; then
    echo "macOS build completed successfully!"
    echo "Executable: build-macos/lizard_meme"
    echo "Don't forget to copy the res/ folder for deployment"
else
    echo "Build failed!"
    exit 1
fi
