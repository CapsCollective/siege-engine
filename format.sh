#!/bin/bash

# Check and assign supplied arguments
if [[ $# -lt 1 ]]; then
  echo "Program requires at least one argument (e.g. format <dirs> [<excludes>] [--check])"
  exit 1
elif [[ $# -eq 2 && "$2" -eq "--check" ]]; then
  check="$2"
elif [[ $# -eq 3 ]]; then
  excludes="-not -iname $2"
  check="$3"
fi;
dirs=$1

# Find all required files
for dir in $dirs; do
  files+="$(find $dir -iname *.h -o -iname *.cpp $excludes) "
done
count=$(wc -w <<< $files)

# Perform format checking for each file
errors=0
for file in $files; do
  diffs=`clang-format $file | diff $file - | wc -l`;
  if [[ $diffs -ne 0 ]]; then
    if [[ $check ]]; then
      echo "$file does not respect formatting (diff: `echo $diffs` lines)"
    else
      echo "Reformatting $file (diff: `echo $diffs` lines)"
      clang-format -i $file
    fi;
    errors=$((errors+1))
  fi;
done

# Output results and exit
if [[ $errors -eq 0 ]]; then
  echo "All formatting checks passed across `echo $count` files"
else
  if [[ $check ]]; then
    echo "`echo $errors` formatting check(s) failed across `echo $count` files"
    exit 1
  else
    echo "Reformatted `echo $errors` of `echo $count` files"
  fi;
fi
exit 0