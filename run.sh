#!/bin/bash

if [ $# -eq 0 ] || ([ $# -eq 1 ] && [[ "$1" =~ [Hh-] ]]); then
  echo "Usage: $0 <required: day> <optional: v for verbose logging>"
  exit
elif [ $# -eq 2 ] && [[ "$2" == "v" ]]; then
  VERBOSE="-D VERBOSE"
fi

WARNINGS="-Wall -Wextra -Werror -Wpedantic -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default -Wundef -Wno-unused"

if g++ -std=c++17 $VERBOSE $WARNINGS "puzzles/$1/main.cpp"; then
  ./a.out
fi

