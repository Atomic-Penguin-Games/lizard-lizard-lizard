cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD_REQUIRED 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(Lizard_Meme)
set(EXECUTABLE lizard_meme)

set(RAYLIB_DIR ${PROJECT_SOURCE_DIR}/../raylib-5.5)
set(RAYLIB_INCLUDE_DIR ${RAYLIB_DIR}/include)
set(RAYLIB_LIB_PATH ${RAYLIB_DIR}/lib/libraylib.a)

set(RES_DIR "${CMAKE_SOURCE_DIR}/res")
set(RES_LINK "${CMAKE_BINARY_DIR}/res")

include_directories(
  ${PROJECT_SOURCE_DIR}/include
  ${RAYLIB_INCLUDE_DIR}
)


file(GLOB all_SRCS
  "${PROJECT_SOURCE_DIR}/include/*.h"
  "${PROJECT_SOURCE_DIR}/src/*.cpp"
  "${PROJECT_SOURCE_DIR}/src/*.c"
)

add_executable(${EXECUTABLE} ${all_SRCS})

target_link_libraries(${EXECUTABLE} PRIVATE 
    ${RAYLIB_LIB_PATH} 
    m pthread dl GL)
target_compile_features(${EXECUTABLE} PRIVATE cxx_std_23)

if(NOT EXISTS "${RES_LINK}")
    if(UNIX OR APPLE)
        execute_process(COMMAND ln -s "${RES_DIR}" "${RES_LINK}")
    elseif(WIN32)
        execute_process(COMMAND cmd /c "mklink /D \"${RES_LINK}\" \"${RES_DIR}\"")
    endif()
endif()
