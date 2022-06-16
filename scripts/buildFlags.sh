#!/bin/bash

if [ $# -lt 3 ]; then
  echo "Requires at least three arguments (e.g. buildFlags.sh <target_dir> <cxxflags> <dirs>)"
  exit 1
fi

TARGET_FILE=$1
CXXFLAGS=$2
DIRS=$3

if [[ -f "$TARGET_FILE" && "`cat $TARGET_FILE`" != "$CXXFLAGS" ]]; then
  for DIR in "$DIRS"
  do
    rm -rf "$DIR"
  done
fi; echo "$CXXFLAGS" | tee "$TARGET_FILE" >/dev/null