#!/bin/bash

usage_message='Usage Options:\n./build.sh -configure\n./build.sh -build\n./build.sh -run'

if [ $# -eq 0 ]
then
  echo -e $usage_message
else
  if [ $1 == '-configure' ]
  then
    cmake --no-warn-unused-cli \
    -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE \
    -DCMAKE_BUILD_TYPE:STRING=Debug \
    -DCMAKE_C_COMPILER:FILEPATH=/usr/bin/gcc \
    -DCMAKE_CXX_COMPILER:FILEPATH=/usr/bin/g++ \
    -S./ -B./build \
    -G "Unix Makefiles"
  elif [ $1 == '-build' ]
  then
    cmake --build ./build --config Debug --target clean -j 26
    cmake --build ./build --config Debug --target all -j 26
  elif [ $1 == '-run' ]
  then
    cmake --build ./build --config Debug --target clean -j 26
    cmake --build ./build --config Debug --target all -j 26
    ./build/gameboy $2 $3 $4 $5 $6
  else
    echo -e $usage_message
  fi
fi