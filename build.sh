#!/bin/bash

# Universal Build Script for Lizard Meme Game
# Supports both desktop and web builds

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Default values
BUILD_TYPE="desktop"
CLEAN=false

# Function to display usage
usage() {
    echo "Usage: $0 [OPTIONS]"
    echo ""
    echo "Options:"
    echo "  -t, --target TARGET    Build target: 'desktop' (default) or 'web'"
    echo "  -c, --clean           Clean build directory before building"
    echo "  -h, --help            Show this help message"
    echo ""
    echo "Examples:"
    echo "  $0                    # Build for desktop"
    echo "  $0 -t web            # Build for web"
    echo "  $0 -t web -c         # Clean web build and rebuild"
}

# Parse command line arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        -t|--target)
            BUILD_TYPE="$2"
            shift 2
            ;;
        -c|--clean)
            CLEAN=true
            shift
            ;;
        -h|--help)
            usage
            exit 0
            ;;
        *)
            echo -e "${RED}Unknown option: $1${NC}"
            usage
            exit 1
            ;;
    esac
done

# Validate build type
if [[ "$BUILD_TYPE" != "desktop" && "$BUILD_TYPE" != "web" ]]; then
    echo -e "${RED}Error: Invalid build target '$BUILD_TYPE'. Use 'desktop' or 'web'.${NC}"
    exit 1
fi

echo -e "${BLUE}===========================================${NC}"
echo -e "${GREEN}Building Lizard Meme Game for $BUILD_TYPE${NC}"
echo -e "${BLUE}===========================================${NC}"

if [[ "$BUILD_TYPE" == "web" ]]; then
    # Web build
    if ! command -v emcc &> /dev/null; then
        echo -e "${RED}Error: emcc not found in PATH.${NC}"
        echo "Please make sure emsdk is installed and activated:"
        echo "  source /path/to/emsdk/emsdk_env.sh"
        exit 1
    fi

    BUILD_DIR="build_web"
    
    # Check for raylib web library
    RAYLIB_WEB_PATH="../raylib-web/src/libraylib.web.a"
    if [ ! -f "$RAYLIB_WEB_PATH" ]; then
        echo -e "${YELLOW}Warning: Raylib web library not found.${NC}"
        echo "You may need to build raylib for web first:"
        echo "  cd ../raylib-web/src && make PLATFORM=PLATFORM_WEB"
    fi
    
    if [[ "$CLEAN" == true ]] && [[ -d "$BUILD_DIR" ]]; then
        echo -e "${YELLOW}Cleaning $BUILD_DIR...${NC}"
        rm -rf "$BUILD_DIR"
    fi
    
    mkdir -p "$BUILD_DIR"
    cd "$BUILD_DIR"
    
    echo -e "${GREEN}Configuring CMake for web...${NC}"
    emcmake cmake .. -DCMAKE_BUILD_TYPE=Release
    
    echo -e "${GREEN}Building...${NC}"
    emmake make -j$(nproc)
    
    echo -e "${GREEN}Web build completed!${NC}"
    echo -e "${YELLOW}Output files in $BUILD_DIR/:${NC}"
    echo "  - lizard_meme.html"
    echo "  - lizard_meme.js"
    echo "  - lizard_meme.wasm"
    echo "  - lizard_meme.data"
    echo ""
    echo -e "${GREEN}To test locally:${NC}"
    echo "  cd $BUILD_DIR"
    echo "  python3 -m http.server 8000"
    echo "  Open: http://localhost:8000/lizard_meme.html"
    
else
    # Desktop build
    BUILD_DIR="build"
    
    if [[ "$CLEAN" == true ]] && [[ -d "$BUILD_DIR" ]]; then
        echo -e "${YELLOW}Cleaning $BUILD_DIR...${NC}"
        rm -rf "$BUILD_DIR"
    fi
    
    mkdir -p "$BUILD_DIR"
    cd "$BUILD_DIR"
    
    echo -e "${GREEN}Configuring CMake for desktop...${NC}"
    cmake .. -DCMAKE_BUILD_TYPE=Release
    
    echo -e "${GREEN}Building...${NC}"
    make -j$(nproc)
    
    echo -e "${GREEN}Desktop build completed!${NC}"
    echo -e "${YELLOW}Executable: $BUILD_DIR/lizard_meme${NC}"
    echo -e "${GREEN}To run: ./$BUILD_DIR/lizard_meme${NC}"
fi
