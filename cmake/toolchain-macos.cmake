# macOS cross-compilation toolchain for osxcross
set(CMAKE_SYSTEM_NAME Darwin)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

# Set the target macOS version
set(CMAKE_OSX_DEPLOYMENT_TARGET "10.15")

# osxcross paths - adjust these based on your osxcross installation
set(OSXCROSS_ROOT $ENV{OSXCROSS_ROOT})
if(NOT OSXCROSS_ROOT)
    set(OSXCROSS_ROOT "/opt/osxcross")
endif()

set(OSXCROSS_TARGET "x86_64-apple-darwin19")
set(OSXCROSS_SDK "MacOSX10.15.sdk")

# Set compilers
set(CMAKE_C_COMPILER ${OSXCROSS_ROOT}/target/bin/${OSXCROSS_TARGET}-clang)
set(CMAKE_CXX_COMPILER ${OSXCROSS_ROOT}/target/bin/${OSXCROSS_TARGET}-clang++)

# Set the sysroot
set(CMAKE_SYSROOT ${OSXCROSS_ROOT}/target/SDK/${OSXCROSS_SDK})

# Set find root paths
set(CMAKE_FIND_ROOT_PATH ${CMAKE_SYSROOT})
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# Compiler flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mmacosx-version-min=10.15")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mmacosx-version-min=10.15")

# Linker flags
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mmacosx-version-min=10.15")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -mmacosx-version-min=10.15")
