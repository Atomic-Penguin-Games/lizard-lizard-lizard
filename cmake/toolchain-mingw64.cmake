# Toolchain file for cross-compiling to Windows using clang with mingw64

set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

# Specify the cross compiler
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

# Target triple for mingw64
set(CMAKE_C_COMPILER_TARGET x86_64-w64-mingw32)
set(CMAKE_CXX_COMPILER_TARGET x86_64-w64-mingw32)

# Compiler flags for mingw64
set(CMAKE_C_FLAGS_INIT "-target x86_64-w64-mingw32")
set(CMAKE_CXX_FLAGS_INIT "-target x86_64-w64-mingw32")

# Find programs
set(CMAKE_FIND_ROOT_PATH /usr/x86_64-w64-mingw32)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# Set the executable suffix
set(CMAKE_EXECUTABLE_SUFFIX ".exe")
