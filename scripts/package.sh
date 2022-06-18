#!/bin/bash

# Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
#
# This code is released under an unmodified zlib license.
# For conditions of distribution and use, please see:
#     https://opensource.org/licenses/Zlib

# Require a minimum number of arguments
if [ $# -lt 4 ]; then
  echo "Requires at least four arguments (e.g. package.sh <package_name> <executable_name> <output_dir> <build_dir> [--env-vars | --excludes])"
  exit 1
fi

# Generates an executable script
generate_script() {
  local _PATH=$1
  local _BODY=$2
  mkdir -p "$(dirname "$_PATH")"
  echo "$_BODY" > "$_PATH"
  chmod +x "$_PATH"
}

# Set app details
PKG_NAME=$1
EXE_NAME=$2
OUTPUT_DIR=$3
BUILD_DIR=$4
PKG_FILE="$OUTPUT_DIR/$PKG_NAME"
RUN_SCRIPT_NAME="run.sh"
while [[ $# -gt 0 ]] ; do
  case $1 in
    --env-vars) ENV_VARS="$2" ;;
    --excludes) EXCLUDES="$2" ;;
  esac
  shift
done

# Find the current OS
if [[ "$(uname)" == "Darwin" ]]; then
  OS="macos"
else
  OS="linux"
fi

# Create the package file and add contents
if [[ "$OS" == "macos" ]]; then
  # Create the app file
  PKG_FILE="$PKG_FILE.app"
  CONTENTS_DIR_NAME="Contents"
  mkdir -p "$PKG_FILE"
  generate_script "$PKG_FILE/$PKG_NAME" "#!/bin/bash
SCRIPT_PATH=\$(dirname \"\$0\")
SCRIPT_PATH=\$(cd \"\$SCRIPT_PATH\" && pwd)
cd \"\$SCRIPT_PATH/Contents\" && ./$RUN_SCRIPT_NAME"
  chmod +x "$PKG_FILE"
else
  # Create the package directory
  mkdir -p "$PKG_FILE"
fi
CONTENTS_DIR="$PKG_FILE/$CONTENTS_DIR_NAME"

# Add the run script
generate_script "$CONTENTS_DIR/$RUN_SCRIPT_NAME" "#!/bin/bash
$ENV_VARS
./$EXE_NAME"

# Copy across all build directory to contents
cp -r "$BUILD_DIR"/* "$CONTENTS_DIR"

# Rename the executable on Linux
if [[ "$OS" == "linux" ]]; then
  mv "$CONTENTS_DIR/$EXE_NAME" "$CONTENTS_DIR/$PKG_NAME"
fi

# Remove all excludes from contents
for EXCLUDE in $EXCLUDES; do
  EXCLUDE_PATH="$CONTENTS_DIR/$EXCLUDE"
  if [[ "$EXCLUDE_PATH" != "$CONTENTS_DIR/" ]]; then
    rm -rf "$EXCLUDE_PATH"
  fi
done