cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD_REQUIRED 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(Lizard_Lizard_Lizard)
set(EXECUTABLE lizard_lizard_lizard)

# Platform detection and raylib configuration
if (EMSCRIPTEN)
    set(PLATFORM "Web")
    set(RAYLIB_DIR ${PROJECT_SOURCE_DIR}/../raylib-web/)
    set(RAYLIB_INCLUDE_DIR ${RAYLIB_DIR}/src)
    set(RAYLIB_LIB_PATH ${RAYLIB_DIR}/src/libraylib.web.a)
    set(USE_SYSTEM_RAYLIB FALSE)
elseif(WIN32)
    set(PLATFORM "Windows")
    if(CMAKE_CROSSCOMPILING)
        # Cross-compiling for Windows
        set(RAYLIB_DIR ${PROJECT_SOURCE_DIR}/../raylib-mingw64)
        set(RAYLIB_INCLUDE_DIR ${RAYLIB_DIR}/include)
        set(RAYLIB_LIB_PATH ${RAYLIB_DIR}/lib/libraylib.a)
        set(USE_SYSTEM_RAYLIB FALSE)
    else()
        # Native Windows build
        set(RAYLIB_DIR ${PROJECT_SOURCE_DIR}/../raylib-5.5)
        set(RAYLIB_INCLUDE_DIR ${RAYLIB_DIR}/include)
        set(RAYLIB_LIB_PATH ${RAYLIB_DIR}/lib/libraylib.a)
        set(USE_SYSTEM_RAYLIB FALSE)
    endif()
elseif(APPLE)
    set(PLATFORM "macOS")
    if(CMAKE_CROSSCOMPILING)
        # Cross-compiling for macOS
        set(RAYLIB_DIR ${PROJECT_SOURCE_DIR}/../raylib-macos)
        set(RAYLIB_INCLUDE_DIR ${RAYLIB_DIR}/include)
        set(RAYLIB_LIB_PATH ${RAYLIB_DIR}/lib/libraylib.a)
        set(USE_SYSTEM_RAYLIB FALSE)
    else()
        # Try to find system-installed raylib first
        find_package(PkgConfig QUIET)
        if(PkgConfig_FOUND)
            pkg_check_modules(RAYLIB QUIET raylib)
        endif()
        
        if(RAYLIB_FOUND)
            # Use system raylib
            set(RAYLIB_INCLUDE_DIR ${RAYLIB_INCLUDE_DIRS})
            set(RAYLIB_LIB_PATH ${RAYLIB_LIBRARIES})
            set(USE_SYSTEM_RAYLIB TRUE)
            message(STATUS "Using system-installed raylib")
        else()
            # Fall back to local raylib
            set(RAYLIB_DIR ${PROJECT_SOURCE_DIR}/../raylib-5.5)
            set(RAYLIB_INCLUDE_DIR ${RAYLIB_DIR}/include)
            set(RAYLIB_LIB_PATH ${RAYLIB_DIR}/lib/libraylib.a)
            set(USE_SYSTEM_RAYLIB FALSE)
            message(STATUS "Using local raylib from ${RAYLIB_DIR}")
        endif()
    endif()
else()
    set(PLATFORM "Desktop")
    # Try to find system-installed raylib first
    find_package(PkgConfig QUIET)
    if(PkgConfig_FOUND)
        pkg_check_modules(RAYLIB QUIET raylib)
    endif()
    
    if(RAYLIB_FOUND)
        # Use system raylib
        set(RAYLIB_INCLUDE_DIR ${RAYLIB_INCLUDE_DIRS})
        set(RAYLIB_LIB_PATH ${RAYLIB_LIBRARIES})
        set(USE_SYSTEM_RAYLIB TRUE)
        message(STATUS "Using system-installed raylib")
    else()
        # Fall back to local raylib
        set(RAYLIB_DIR ${PROJECT_SOURCE_DIR}/../raylib-5.5)
        set(RAYLIB_INCLUDE_DIR ${RAYLIB_DIR}/include)
        set(RAYLIB_LIB_PATH ${RAYLIB_DIR}/lib/libraylib.a)
        set(USE_SYSTEM_RAYLIB FALSE)
        message(STATUS "Using local raylib from ${RAYLIB_DIR}")
    endif()
endif()

set(RES_DIR "${CMAKE_SOURCE_DIR}/res")
set(RES_LINK "${CMAKE_BINARY_DIR}/res")

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include/)

# Add raylib include directories if not using system raylib
if(NOT USE_SYSTEM_RAYLIB)
    include_directories(${RAYLIB_INCLUDE_DIR})
endif()

file(GLOB all_SRCS
  "${PROJECT_SOURCE_DIR}/include/*.h"
  "${PROJECT_SOURCE_DIR}/src/*.cpp"
  "${PROJECT_SOURCE_DIR}/src/*.c"
)

add_executable(${EXECUTABLE} ${all_SRCS})

# Platform-specific configurations
if (EMSCRIPTEN)
    # Web build settings
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s GL_ENABLE_GET_PROC_ADDRESS=1")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --shell-file ${CMAKE_SOURCE_DIR}/web/shell.html")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --preload-file ${RES_DIR}@/res/")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s INITIAL_MEMORY=134217728")  # 128MB
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s ALLOW_MEMORY_GROWTH=1")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s EXPORTED_FUNCTIONS=['_main','_malloc','_free']")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s EXPORTED_RUNTIME_METHODS=['ccall','cwrap','requestFullscreen']")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s FORCE_FILESYSTEM=1")
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    
    target_link_libraries(${EXECUTABLE} PRIVATE ${RAYLIB_LIB_PATH})
elseif(APPLE)
    # macOS build settings
    if(CMAKE_CROSSCOMPILING)
        # Cross-compiling for macOS with osxcross
        target_link_libraries(${EXECUTABLE} PRIVATE 
            ${RAYLIB_LIB_PATH}
            "-framework OpenGL" 
            "-framework Cocoa" 
            "-framework IOKit" 
            "-framework CoreVideo")
        
        # Copy resources to build directory for macOS
        if(NOT EXISTS "${RES_LINK}")
            execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory "${RES_DIR}" "${RES_LINK}")
        endif()
    else()
        # Native macOS build
        if(USE_SYSTEM_RAYLIB)
            # System raylib with pkg-config
            target_link_libraries(${EXECUTABLE} PRIVATE 
                ${RAYLIB_LIB_PATH}
                "-framework OpenGL" 
                "-framework Cocoa" 
                "-framework IOKit" 
                "-framework CoreVideo")
            target_include_directories(${EXECUTABLE} PRIVATE ${RAYLIB_INCLUDE_DIR})
        else()
            # Local raylib
            target_link_libraries(${EXECUTABLE} PRIVATE 
                ${RAYLIB_LIB_PATH}
                "-framework OpenGL" 
                "-framework Cocoa" 
                "-framework IOKit" 
                "-framework CoreVideo")
        endif()
        
        # Create symlink for resources (native macOS)
        if(NOT EXISTS "${RES_LINK}")
            execute_process(COMMAND ln -s "${RES_DIR}" "${RES_LINK}")
        endif()
    endif()
    
    target_compile_features(${EXECUTABLE} PRIVATE cxx_std_23)
elseif(WIN32)
    # Windows build settings
    if(CMAKE_CROSSCOMPILING)
        # Cross-compiling for Windows with mingw64
        target_link_libraries(${EXECUTABLE} PRIVATE 
            ${RAYLIB_LIB_PATH}
            -lwinmm -lgdi32 -lopengl32)
        
        # Copy resources to build directory for Windows
        if(NOT EXISTS "${RES_LINK}")
            execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory "${RES_DIR}" "${RES_LINK}")
        endif()
    else()
        # Native Windows build
        target_link_libraries(${EXECUTABLE} PRIVATE 
            ${RAYLIB_LIB_PATH}
            winmm gdi32 opengl32)
        
        # Copy resources for native Windows build
        if(NOT EXISTS "${RES_LINK}")
            execute_process(COMMAND cmd /c "xcopy \"${RES_DIR}\" \"${RES_LINK}\" /E /I /Y")
        endif()
    endif()
    
    target_compile_features(${EXECUTABLE} PRIVATE cxx_std_23)
else()
    # Desktop build settings (Linux only now)
    if(USE_SYSTEM_RAYLIB)
        # System raylib with pkg-config
        target_link_libraries(${EXECUTABLE} PRIVATE 
            ${RAYLIB_LIB_PATH} 
            m pthread dl GL)
        target_include_directories(${EXECUTABLE} PRIVATE ${RAYLIB_INCLUDE_DIR})
    else()
        # Local raylib
        target_link_libraries(${EXECUTABLE} PRIVATE 
            ${RAYLIB_LIB_PATH} 
            m pthread dl GL)
    endif()
    target_compile_features(${EXECUTABLE} PRIVATE cxx_std_23)
    
    # Create symlink for resources (Linux)
    if(NOT EXISTS "${RES_LINK}")
        execute_process(COMMAND ln -s "${RES_DIR}" "${RES_LINK}")
    endif()
endif()
