#!/bin/bash

# Web Build Script for Lizard Meme Game
# Requires emsdk to be installed and activated

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${GREEN}Building Lizard Meme Game for Web...${NC}"

# Check if emsdk is in PATH
if ! command -v emcc &> /dev/null; then
    echo -e "${RED}Error: emcc not found in PATH. Please make sure emsdk is installed and activated.${NC}"
    echo "Run: source /path/to/emsdk/emsdk_env.sh"
    exit 1
fi

# Check if raylib web version exists
RAYLIB_WEB_PATH="../raylib-web/src/libraylib.web.a"
if [ ! -f "$RAYLIB_WEB_PATH" ]; then
    echo -e "${YELLOW}Warning: Raylib web library not found at $RAYLIB_WEB_PATH${NC}"
    echo "You may need to build raylib for web first:"
    echo "cd ../raylib-web/src && make PLATFORM=PLATFORM_WEB"
fi

# Create web build directory
WEB_BUILD_DIR="build_web"
if [ -d "$WEB_BUILD_DIR" ]; then
    echo -e "${YELLOW}Cleaning existing web build directory...${NC}"
    rm -rf "$WEB_BUILD_DIR"
fi

mkdir -p "$WEB_BUILD_DIR"
cd "$WEB_BUILD_DIR"

echo -e "${GREEN}Configuring CMake for web build...${NC}"

# Configure with emcmake
emcmake cmake .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_TOOLCHAIN_FILE=$EMSDK/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake

echo -e "${GREEN}Building web version...${NC}"

# Build with emmake
emmake make -j$(nproc)

echo -e "${GREEN}Web build completed!${NC}"
echo -e "${YELLOW}Output files:${NC}"
echo "  - lizard_meme.html (main file to open in browser)"
echo "  - lizard_meme.js"
echo "  - lizard_meme.wasm"
echo "  - lizard_meme.data"
echo ""
echo -e "${GREEN}To test locally, run a web server in the build_web directory:${NC}"
echo "  cd build_web"
echo "  python3 -m http.server 8000"
echo "  Then open http://localhost:8000/lizard_meme.html"
