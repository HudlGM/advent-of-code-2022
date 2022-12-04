#!/bin/bash

if [ $# -eq 0 ] || ([ $# -eq 1 ] && [[ "$1" =~ [Hh-] ]]); then
  echo "Usage: $0 <required: day> <optional: v for verbose logging>"
  exit
elif [ $# -eq 2 ] && [[ "$2" == "v" ]]; then
  VERBOSE="-D VERBOSE"
fi

if g++ $VERBOSE -Wall -Wextra -Werror -Wpedantic "puzzles/$1/main.cpp"; then
  ./a.out
fi

