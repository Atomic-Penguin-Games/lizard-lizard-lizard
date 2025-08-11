@echo off
echo Building Lizard Meme Game for Windows (native)...

if exist build-windows-native (
    echo Cleaning existing Windows native build directory...
    rmdir /s /q build-windows-native
)

mkdir build-windows-native
cd build-windows-native

echo Configuring CMake for Windows native build...
cmake .. -DCMAKE_BUILD_TYPE=Release

echo Building the project...
cmake --build . --config Release

echo Windows native build complete!
echo Executable: build-windows-native\Release\lizard_meme.exe

pause
