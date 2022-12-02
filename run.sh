#!/bin/bash

if [ $# -eq 0 ]; then
  echo "Usage: $0 <required: day> <optional: v for verbose logging>"
  exit
elif [ $# -eq 2 ] && [ $2 == "v" ]; then
  VERBOSE="-D VERBOSE"
fi

if g++ $VERBOSE -Wall $1.cpp; then
  ./a.out
fi

