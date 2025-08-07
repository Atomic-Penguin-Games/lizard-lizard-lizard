cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD_REQUIRED 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(Lizard_Meme)
set(EXECUTABLE lizard_meme)

# Platform detection
if (EMSCRIPTEN)
    set(PLATFORM "Web")
    set(RAYLIB_DIR ${PROJECT_SOURCE_DIR}/../raylib-web/)
    set(RAYLIB_INCLUDE_DIR ${RAYLIB_DIR}/src)
    set(RAYLIB_LIB_PATH ${RAYLIB_DIR}/src/libraylib.web.a)
else()
    set(PLATFORM "Desktop")
    set(RAYLIB_DIR ${PROJECT_SOURCE_DIR}/../raylib-5.5)
    set(RAYLIB_INCLUDE_DIR ${RAYLIB_DIR}/include)
    set(RAYLIB_LIB_PATH ${RAYLIB_DIR}/lib/libraylib.a)
endif()

set(RES_DIR "${CMAKE_SOURCE_DIR}/res")
set(RES_LINK "${CMAKE_BINARY_DIR}/res")

include_directories(
  ${PROJECT_SOURCE_DIR}/include/
  ${RAYLIB_INCLUDE_DIR}
)

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
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s TOTAL_MEMORY=67108864")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s FORCE_FILESYSTEM=1")
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    
    target_link_libraries(${EXECUTABLE} PRIVATE ${RAYLIB_LIB_PATH})
else()
    # Desktop build settings
    target_link_libraries(${EXECUTABLE} PRIVATE 
        ${RAYLIB_LIB_PATH} 
        m pthread dl GL)
    target_compile_features(${EXECUTABLE} PRIVATE cxx_std_23)
    
    # Create symlink for resources (desktop only)
    if(NOT EXISTS "${RES_LINK}")
        if(UNIX OR APPLE)
            execute_process(COMMAND ln -s "${RES_DIR}" "${RES_LINK}")
        elseif(WIN32)
            execute_process(COMMAND cmd /c "mklink /D \"${RES_LINK}\" \"${RES_DIR}\"")
        endif()
    endif()
endif()
