#!/bin/bash
#
# Copyright (c) 2020-present Caps Collective & contributors
# Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
#
# This code is released under an unmodified zlib license.
# For conditions of distribution and use, please see:
#     https://opensource.org/licenses/Zlib

LIBS=""; OBJS=""; OUTPUT_NAME=""; OUTPUT_DIR=""; CURRENT_ARG=""

for i in "$@"; do
  if [[ "$i" == *"--"* ]]; then
    if [[ "$i" != "$CURRENT_ARG" ]]; then CURRENT_ARG=$i; fi; continue
  fi

  if [ "$CURRENT_ARG" == "--src_libs" ]; then LIBS+="$i "; fi
  if [ "$CURRENT_ARG" == "--src_objs" ]; then OBJS+="$i "; fi
  if [ "$CURRENT_ARG" == "--output_name" ]; then OUTPUT_NAME="$i"; fi
  if [ "$CURRENT_ARG" == "--output_dir" ]; then OUTPUT_DIR="$i"; fi
done

mkdir -p "$OUTPUT_DIR"

ar -crs "$OUTPUT_DIR/lib$OUTPUT_NAME.a" ${OBJS}

IFS=' ' read -ra SPLIT_LIBS <<< "$LIBS"
for i in "${SPLIT_LIBS[@]}"; do (cd "$OUTPUT_DIR" && ar -x "$i"); done

ar -q "$OUTPUT_DIR"/lib"$OUTPUT_NAME".a "$OUTPUT_DIR"/*.o

(shopt -s extglob; glob='*.!(a)'; rm ${OUTPUT_DIR}/${glob})