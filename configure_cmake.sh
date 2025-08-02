#!/bin/bash

file=CMakeLists.txt
file2=.vscode/launch.json
file3=src/main.c
projectName="${1}"
cmakeProject="${projectName// /_}"
executableName=$(echo "$projectName" | tr '[:upper:]' '[:lower:]' | sed 's/ /_/g' | sed 's/[^a-z0-9_]//g')

sed -i -e "s/{#projectName}/$cmakeProject/g" -e "s/{#executableName}/$executableName/g" "$file"
sed -i -e "s/{#executableName}/$executableName/g" "$file2"
sed -i -e "s/{#gameName}/$projectName/g" "$file3"

mkdir res include
cmake -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -S . -B build
