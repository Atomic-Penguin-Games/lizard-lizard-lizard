# Build Instructions

This project supports building for multiple platforms: Linux/Desktop, Windows (cross-compilation), and Web (Emscripten).

## Prerequisites

### For Linux/Desktop builds:
- CMake 3.13 or higher
- GCC or Clang compiler
- Raylib 5.5 development libraries
- OpenGL development libraries

### For Windows cross-compilation:
- Clang with mingw64 target support
- MinGW-w64 toolchain
- Raylib built for mingw64

### For Web builds:
- Emscripten SDK
- Raylib built for web

## Building

### Linux/Desktop Build
```bash
mkdir build
cd build
cmake ..
make
```

### Windows Cross-Compilation Build
```bash
./build-windows.sh
```

Or manually:
```bash
mkdir build-windows
cd build-windows
cmake .. -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchain-mingw64.cmake -DCMAKE_BUILD_TYPE=Release
make
```

### Web Build
```bash
source /path/to/emsdk/emsdk_env.sh  # Activate Emscripten
mkdir build-web
cd build-web
emcmake cmake .. -DCMAKE_BUILD_TYPE=Release
emmake make
```

## Raylib Setup

### For Windows Cross-Compilation
You'll need Raylib compiled for mingw64. Place it in a directory structure like:
```
../raylib-mingw64/
├── include/
│   └── raylib.h
└── lib/
    └── libraylib.a
```

### For Web
You'll need Raylib compiled for Emscripten. Place it in:
```
../raylib-web/
├── src/
│   ├── raylib.h
│   └── libraylib.web.a
```

## Output

- **Linux**: `build/lizard_meme`
- **Windows**: `build-windows/lizard_meme.exe`
- **Web**: `build-web/lizard_meme.html`

## Distribution

### Windows
Copy the following files for distribution:
- `lizard_meme.exe`
- `res/` directory (contains game assets)

### Web
Copy the following files to your web server:
- `lizard_meme.html`
- `lizard_meme.js`
- `lizard_meme.wasm`
- `lizard_meme.data` (contains game assets)
