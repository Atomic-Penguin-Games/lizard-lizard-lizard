# Raylib-clang-template

##
This is simply a preconfigured Cmake project setup to build raylib games written in C/C++ with Clang.

The script expects your raylib libraries to be in a folder in the same directory your project is in, with a lib and include folder.

- Dev/
  - raylib/
    - include/
    - lib/
  - myGameProject/
    - CMakeLists.txt
    - configure_cmake.sh
    - src/

## Usage
run `./configure_cmake.sh 'GameName'
