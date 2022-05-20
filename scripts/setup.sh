#!/bin/bash

# Find repo root
SCRIPT_DIR=$(dirname "${BASH_SOURCE[0]}")
cd "$SCRIPT_DIR/.." || exit
ROOT_DIR=$(pwd -P)

# Find the current OS
if [ "$(uname)" == "Darwin" ]; then
  OS="macos"
  NUMBER_OF_PROCESSORS="$(sysctl -n hw.ncpu)"
else
	OS="linux"
	NUMBER_OF_PROCESSORS="$(nproc)"
fi

# Set setup details
GENERATOR="Unix Makefiles"
VENDOR_DIR="${ROOT_DIR}/vendor"

update_submodules() {
  git submodule update --init ${VENDOR_DIR}/$1
}

setup_glfw() {
  update_submodules glfw

  cmake -G "${GENERATOR}" -B${VENDOR_DIR}/glfw -S${VENDOR_DIR}/glfw
  make -C ${VENDOR_DIR}/glfw -j${NUMBER_OF_PROCESSORS}
}

setup_glslang() {
  update_submodules glslang

  mkdir -p ${VENDOR_DIR}/glslang/build

  cmake -DCMAKE_INSTALL_PREFIX=${VENDOR_DIR}/glslang/build -G "${GENERATOR}" -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=${VENDOR_DIR}/glslang/build/install -S${VENDOR_DIR}/glslang -B${VENDOR_DIR}/glslang/build
  cmake --build ${VENDOR_DIR}/glslang/build --target install -- -j${NUMBER_OF_PROCESSORS}
}

echo "OS detected: ${OS}"

echo "Setting up GLFW..."
setup_glfw

echo "Setting up GLSLang..."
setup_glslang

VULKAN_LIB_DIR="${VENDOR_DIR}/vulkan/lib/${OS}"
echo "Configuring environment file..."
{
  echo "DYLD_LIBRARY_PATH='${VULKAN_LIB_DIR}'"
} > .env

if [[ "${OS}" == "macos" ]]; then
  echo "VK_ICD_FILENAMES='${VULKAN_LIB_DIR}/icd.d/MoltenVK_icd.json'" >> .env
fi

if [[ "$1" == "DEBUG" ]]; then
  echo "VK_LAYER_PATH='${VULKAN_LIB_DIR}/explicit_layer.d'" >> .env
fi