#!/bin/bash

# Test Windows cross-compilation locally
# This will help debug the Windows build issues

echo "Testing Windows cross-compilation..."

# Check if mingw64 toolchain exists
if [ ! -f "cmake/toolchain-mingw64.cmake" ]; then
    echo "❌ Error: cmake/toolchain-mingw64.cmake not found"
    exit 1
fi

echo "✅ Toolchain file found"

# Test if we can find the cross-compiler
CROSS_COMPILER=$(grep CMAKE_C_COMPILER cmake/toolchain-mingw64.cmake | cut -d'"' -f2)
echo "Looking for cross-compiler: $CROSS_COMPILER"

if command -v "$CROSS_COMPILER" >/dev/null 2>&1; then
    echo "✅ Cross-compiler found: $CROSS_COMPILER"
    $CROSS_COMPILER --version
else
    echo "❌ Cross-compiler not found: $CROSS_COMPILER"
    echo "Install with: sudo apt-get install mingw-w64"
fi

# Check if we can build a simple test
echo ""
echo "Testing CMake configuration..."

mkdir -p test-build-windows
cd test-build-windows

# Create a simple test file
cat > test.c << 'EOF'
#include <stdio.h>
int main() {
    printf("Hello from Windows cross-compilation!\n");
    return 0;
}
EOF

# Create a simple CMakeLists.txt
cat > CMakeLists.txt << 'EOF'
cmake_minimum_required(VERSION 3.13)
project(test)
add_executable(test test.c)
EOF

# Try to configure
echo "Configuring with Windows toolchain..."
if cmake . -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchain-mingw64.cmake; then
    echo "✅ CMake configuration successful"
    
    if make; then
        echo "✅ Build successful"
        file test.exe
    else
        echo "❌ Build failed"
    fi
else
    echo "❌ CMake configuration failed"
fi

cd ..
rm -rf test-build-windows

echo ""
echo "Test completed. If this works locally, the GitHub Actions issue might be with the raylib build step."
