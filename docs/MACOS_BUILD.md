# macOS Cross-Compilation Setup Guide

This guide explains how to build your raylib game for macOS from a Linux system.

## Prerequisites

### 1. Install osxcross
osxcross is a cross-compilation toolchain for macOS targets.

```bash
# Install dependencies
sudo apt-get update
sudo apt-get install cmake clang llvm-dev liblzma-dev libxml2-dev uuid-dev libssl-dev bash patch make tar xz-utils bzip2 gzip sed cpio libbz2-dev

# Clone osxcross
git clone https://github.com/tpoechtrager/osxcross.git
cd osxcross

# Download macOS SDK (you need to get this legally)
# Place MacOSX10.15.sdk.tar.xz in osxcross/tarballs/
# You can extract this from Xcode or get it from a macOS system

# Build osxcross
UNATTENDED=1 ./build.sh

# Install to /opt/osxcross (optional but recommended)
sudo mv target /opt/osxcross
```

### 2. Build raylib for macOS
You need a macOS-compiled version of raylib:

```bash
# Go to your raylib source directory
cd /path/to/raylib-source

# Create build directory for macOS
mkdir build-macos
cd build-macos

# Configure with osxcross toolchain
cmake .. \
    -DCMAKE_TOOLCHAIN_FILE=/path/to/your/game/cmake/toolchain-macos.cmake \
    -DCMAKE_BUILD_TYPE=Release \
    -DBUILD_EXAMPLES=OFF \
    -DBUILD_GAMES=OFF

# Build raylib
make -j$(nproc)

# Install to a directory next to your game
make install DESTDIR=../../raylib-macos
```

## Directory Structure
After setup, your directory structure should look like:

```
/home/piccolo/Dev/games/raylib/
├── lizard-meme-game/          # Your game
├── raylib-5.5/                # Linux raylib
├── raylib-mingw64/            # Windows raylib
├── raylib-macos/              # macOS raylib (new)
└── raylib-web/                # Web raylib
```

## Building for macOS

Once everything is set up:

```bash
# From your game directory
./build-macos.sh
```

## macOS SDK Legal Notice

**Important**: You need to obtain the macOS SDK legally. This typically means:
1. You own a Mac and can extract the SDK from Xcode
2. You have a valid Apple Developer account
3. You extract it from your own macOS system

The SDK file should be named `MacOSX10.15.sdk.tar.xz` and placed in the osxcross tarballs directory.

## Alternative: GitHub Actions

If setting up osxcross is too complex, consider using GitHub Actions for macOS builds:

```yaml
name: Build macOS
on: [push, pull_request]
jobs:
  build-macos:
    runs-on: macos-latest
    steps:
    - uses: actions/checkout@v3
    - name: Install raylib
      run: brew install raylib
    - name: Build
      run: |
        mkdir build
        cd build
        cmake .. -DCMAKE_BUILD_TYPE=Release
        make
```

## Universal Binaries (Intel + Apple Silicon)

For universal macOS binaries that work on both Intel and Apple Silicon Macs, you'll need to:

1. Build for both architectures
2. Use `lipo` to combine them

This requires more complex toolchain setup and is typically done on actual macOS systems.

## Deployment

For distribution on macOS:
1. Code signing (requires Apple Developer account)
2. Notarization (for Gatekeeper)
3. Creating .app bundles
4. DMG creation for distribution

These steps are typically done on macOS systems or through automated CI/CD pipelines.
