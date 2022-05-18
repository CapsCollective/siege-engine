#!/bin/bash

# Require four arguments for package name, output path, binary path, and debug state
if [ $# -ne 4 ]; then
  echo "Requires four arguments (e.g. packageapp.sh <package_name> <output_dir> <bin_dir> <debug>)"
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
OUTPUT_DIR=$2
BIN_DIR=$3
DEBUG=$4
PKG_FILE="$OUTPUT_DIR/$PKG_NAME"
RUN_SCRIPT_NAME="run.sh"

# Find the current OS
if [ "$(uname)" == "Darwin" ]; then
  OS="macos"
else
	OS="linux"
fi

# Create the package file and add contents
if [[ "$OS" == "macos" ]]; then
  # Create the app file
  PKG_FILE="$PKG_FILE.app"
  CONTENTS_DIR_NAME="Contents"
  EXE_FILE_NAME="app"
  mkdir -p "$PKG_FILE"
  generate_script "$PKG_FILE/$PKG_NAME" "#!/bin/bash
SCRIPT_PATH=\$(dirname \"\$0\")
SCRIPT_PATH=\$(cd \"\$SCRIPT_PATH\" && pwd)
cd \"\$SCRIPT_PATH/Contents\" && ./$RUN_SCRIPT_NAME"
  ICD_FILENAMES="
export VK_ICD_FILENAMES='./lib/icd.d/MoltenVK_icd.json'"
else
  # Create the package directory
  EXE_FILE_NAME="$PKG_NAME"
  mkdir -p "$PKG_FILE"
fi
CONTENTS_DIR="$PKG_FILE/$CONTENTS_DIR_NAME"
EXE_FILE="$CONTENTS_DIR/$EXE_FILE_NAME"
LIB_DIR="$CONTENTS_DIR/lib"

# Add the run script
generate_script "$CONTENTS_DIR/$RUN_SCRIPT_NAME" "#!/bin/bash
export DYLD_LIBRARY_PATH='./lib'
export VK_LAYER_PATH='./lib/explicit_layer.d'$ICD_FILENAMES
./$EXE_FILE_NAME"

# Copy across the executable, Vulkan binaries and shaders
cp -r "$BIN_DIR/lib" "$CONTENTS_DIR"
if [[ "$DEBUG" -ne "1" ]]; then
  rm "$LIB_DIR/libVkLayer_khronos_validation."*
  rm -rf "$LIB_DIR/explicit_layer.d"
fi
cp -r "$BIN_DIR/shaders" "$CONTENTS_DIR"
cp -r "$BIN_DIR/app" "$EXE_FILE"