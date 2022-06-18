#!/bin/bash

# Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
#
# This code is released under an unmodified zlib license.
# For conditions of distribution and use, please see:
#     https://opensource.org/licenses/Zlib

# Check and assign supplied arguments
if [[ $# -lt 1 ]]; then
  echo "Program requires at least one argument (e.g. format <dirs> [--check])"
  exit 1
fi
dirs=$1
while [[ $# -gt 0 ]] ; do
  case $1 in
    --check) check="1" ;;
  esac
  shift
done

# Remove whitespace
strips() {
  echo "$1" | xargs
}

# Find all required files
for dir in $dirs; do
  files+="$(find "$dir" -iname "*.h" -o -iname "*.cpp") "
done
count=$(wc -w <<< "$files")

# Perform format checking for each file
errors=0
for file in $files; do
  diffs=$(clang-format "$file" | diff "$file" - | wc -l);
  if [[ $diffs -ne 0 ]]; then
    if [[ $check ]]; then
      echo "$file does not respect formatting (diff: $(strips "$diffs") lines)"
    else
      echo "Reformatting $file (diff: $(strips "$diffs") lines)"
      clang-format -i "$file"
    fi
    errors=$((errors+1))
  fi
done

# Output results and exit
if [[ $errors -eq 0 ]]; then
  echo "All formatting checks passed across $(strips "$count") files"
else
  if [[ $check ]]; then
    echo "$(strips "$errors") formatting check(s) failed across $(strips "$count") files"
    exit 1
  else
    echo "Reformatted $(strips "$errors") of $(strips "$count") files"
  fi
fi
exit 0