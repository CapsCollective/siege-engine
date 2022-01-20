#!/bin/bash

# Check supplied arguments
if [ $# -lt 2 ]; then
  echo "Program requires at least two argument (e.g. format <dirs> <excludes> [--check])"
  exit 1
fi;

# Find all required files
dirs=$1
excludes=$2
check="$3"
for dir in $dirs; do
  files+="$(find $dir -iname *.h -o -iname *.cpp -not -iname $excludes) "
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