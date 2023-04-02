#!/bin/bash

# Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
#
# This code is released under an unmodified zlib license.
# For conditions of distribution and use, please see:
#     https://opensource.org/licenses/Zlib

# Find repo root
SCRIPT_DIR=$(dirname "${BASH_SOURCE[0]}")
cd "$SCRIPT_DIR/.." || exit
ROOT_DIR=$(pwd -P)

# Find the current OS
if [ "$(uname)" == "Darwin" ]; then
    OS="macos"
    NUMBER_OF_PROCESSORS="$(sysctl -n hw.ncpu)"
    LIB_SUFFIX=".dylib"
    LOADER_INSTALL_DIR=build/layers
else
    OS="linux"
    NUMBER_OF_PROCESSORS="$(nproc)"
    LIB_SUFFIX=".so"
    LOADER_INSTALL_DIR=build/share/vulkan/explicit_layer.d
fi

# Set setup details
VULKAN_VERSION="v1.3.231"
SPIRV_VERSION="2022.4"
GENERATOR="Unix Makefiles"
VENDOR_DIR="${ROOT_DIR}/vendor"

# Vulkan dependency variables
VULKAN_VENDOR_DIR="${VENDOR_DIR}/vulkan"
VULKAN_LIB_DIR="${VULKAN_VENDOR_DIR}/lib"
VULKAN_INCLUDE_DIR="${VULKAN_VENDOR_DIR}/include/vulkan"
VOLK_INCLUDE_DIR="${VULKAN_VENDOR_DIR}/include/volk"

update_submodules() {
    git submodule update --init "${VENDOR_DIR}"/"$1"
}

checkout_tags() {
    git -C "$1"  fetch --all --tags ; git -C "$1" checkout tags/"$2"
}

setup_glfw() {
    echo "Cloning GLFW..."
    update_submodules glfw

    echo "Setting up GLFW..."
    cmake -G "${GENERATOR}" -B"${VENDOR_DIR}"/glfw -S"${VENDOR_DIR}"/glfw
    make -C "${VENDOR_DIR}"/glfw -j"${NUMBER_OF_PROCESSORS}"
}

setup_glslang() {
    echo "Cloning GLSLang..."
    update_submodules glslang

    echo "Setting up GLSLang..."
    mkdir -p "${VENDOR_DIR}"/glslang/build
    cmake \
        -DCMAKE_INSTALL_PREFIX="${VENDOR_DIR}"/glslang/build \
        -G "${GENERATOR}" -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_INSTALL_PREFIX="${VENDOR_DIR}"/glslang/build/install \
        -S"${VENDOR_DIR}"/glslang \
        -B"${VENDOR_DIR}"/glslang/build
    cmake --build "${VENDOR_DIR}"/glslang/build --target install -- -j"${NUMBER_OF_PROCESSORS}"
}

setup_glm() {
  echo "Cloning GLM..."
  update_submodules glm
}

setup_volk() {
    echo "Cloning Volk..."
    update_submodules vulkan/volk

    echo "Setting up Volk..."
    mkdir -p "${VOLK_INCLUDE_DIR}"
    cp "${VULKAN_VENDOR_DIR}"/volk/volk.h "${VOLK_INCLUDE_DIR}"
    cp "${VULKAN_VENDOR_DIR}"/volk/volk.c "${VOLK_INCLUDE_DIR}"
}

setup_vulkan_headers() {
    echo "Cloning Vulkan Headers..."
    update_submodules vulkan/Vulkan-Headers
    checkout_tags "${VULKAN_VENDOR_DIR}"/Vulkan-Headers "$VULKAN_VERSION"

    echo "Setting up Vulkan Headers..."
    local BUILD_DIR="${VULKAN_VENDOR_DIR}"/Vulkan-Headers/build
    mkdir -p "${BUILD_DIR}"
    cmake -DCMAKE_INSTALL_PREFIX="${BUILD_DIR}"/install -G "${GENERATOR}" -B"${BUILD_DIR}" -S"${VULKAN_VENDOR_DIR}"/Vulkan-Headers
    cmake --build "${BUILD_DIR}" --target install
    mkdir -p "${VULKAN_INCLUDE_DIR}"
    cp "${VULKAN_VENDOR_DIR}"/Vulkan-Headers/include/vulkan/**.h "${VULKAN_INCLUDE_DIR}"
}

setup_vulkan_loader() {
    echo "Cloning Vulkan Loader..."
    update_submodules vulkan/Vulkan-Loader
    checkout_tags "${VULKAN_VENDOR_DIR}"/Vulkan-Loader "$VULKAN_VERSION"
    
    echo "Setting up Vulkan Loader..."
    local BUILD_DIR="${VULKAN_VENDOR_DIR}"/Vulkan-Loader/build
    mkdir -p "${BUILD_DIR}"
    cmake \
        -DCMAKE_INSTALL_PREFIX="${BUILD_DIR}" \
        -DVULKAN_HEADERS_INSTALL_DIR="${VULKAN_VENDOR_DIR}"/Vulkan-Headers/build/install \
        -S"${VULKAN_VENDOR_DIR}"/Vulkan-Loader \
        -B"${BUILD_DIR}"
    cmake --build "${BUILD_DIR}" --config Release
    mkdir -p "${VULKAN_LIB_DIR}"
    cp "${BUILD_DIR}"/loader/**"${LIB_SUFFIX}" "${VULKAN_LIB_DIR}"
}

setup_moltenVk() {
    echo "Cloning MoltenVk..."
    update_submodules vulkan/MoltenVK
    checkout_tags "${VULKAN_VENDOR_DIR}"/MoltenVK "v1.2.0"

    echo "Setting up MoltenVk..."
    (cd "${VULKAN_VENDOR_DIR}"/MoltenVK ; ./fetchDependencies --macos --v-headers-root "${VULKAN_VENDOR_DIR}"/Vulkan-Headers)
    make -C "${VULKAN_VENDOR_DIR}"/MoltenVK macos -j"${NUMBER_OF_PROCESSORS}"
    mkdir -p "${VULKAN_LIB_DIR}"/icd.d
    mkdir -p "${VULKAN_INCLUDE_DIR}"
    cp "${VULKAN_VENDOR_DIR}"/MoltenVK/Package/Latest/MoltenVK/dylib/macOS/** "${VULKAN_LIB_DIR}"/icd.d
}

setup_robin_hood_hashing() {
    echo "Cloning Robin Hood Hashing..."
    update_submodules vulkan/robin-hood-hashing

    echo "Setting up Robin Hood Hashing..."
    local BUILD_DIR="${VULKAN_VENDOR_DIR}"/robin-hood-hashing/build
    cmake \
        -G "${GENERATOR}" "${VULKAN_VENDOR_DIR}"/robin-hood-hashing \
        -DCMAKE_INSTALL_PREFIX="${BUILD_DIR}"/install \
        -DRH_STANDALONE_PROJECT=OFF \
        -DCMAKE_BUILD_TYPE=Release \
        -S"${VULKAN_VENDOR_DIR}"/robin-hood-hashing \
        -B"${BUILD_DIR}"
    cmake --build "${BUILD_DIR}" --target install -- -j"${NUMBER_OF_PROCESSORS}"
}

setup_spirv_headers() {
    echo "Cloning SPIRV Headers..."
    update_submodules vulkan/SPIRV-Headers

    checkout_tags "${VULKAN_VENDOR_DIR}"/SPIRV-Headers "sdk-1.3.231.1"

    echo "Setting up SPIRV Headers..."
    local BUILD_DIR="${VULKAN_VENDOR_DIR}"/SPIRV-Headers/build
    mkdir -p "${BUILD_DIR}"
    cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="${BUILD_DIR}"/install -S"${VULKAN_VENDOR_DIR}"/SPIRV-Headers -B"${BUILD_DIR}"
    cmake --build "${BUILD_DIR}" --target install --config Release
}

setup_spirv_tools() {
    echo "Cloning SPIRV Tools..."
    update_submodules vulkan/SPIRV-Tools
    checkout_tags "${VULKAN_VENDOR_DIR}"/SPIRV-Tools "$SPIRV_VERSION"

    echo "Setting up SPIRV Tools..."
    local BUILD_DIR="${VULKAN_VENDOR_DIR}"/SPIRV-Tools/build
    mkdir -p "${BUILD_DIR}"
    python3 "${VULKAN_VENDOR_DIR}"/SPIRV-Tools/utils/git-sync-deps
    cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="${BUILD_DIR}"/install -S"${VULKAN_VENDOR_DIR}"/SPIRV-Tools -B"${BUILD_DIR}"
    cmake --build "${BUILD_DIR}" --target install --config Release -j"${NUMBER_OF_PROCESSORS}"
}

setup_validation_layers() {
    echo "Cloning Vulkan Validation Layers..."
    update_submodules vulkan/Vulkan-ValidationLayers
    checkout_tags "${VULKAN_VENDOR_DIR}"/Vulkan-ValidationLayers "$VULKAN_VERSION"

    echo "Setting up Vulkan Validation Layers..."
    local BUILD_DIR="${VULKAN_VENDOR_DIR}"/Vulkan-ValidationLayers/build
    mkdir -p "${BUILD_DIR}"
    cmake \
        -DVULKAN_HEADERS_INSTALL_DIR="${VULKAN_VENDOR_DIR}"/Vulkan-Headers/build/install          \
        -DVULKAN_LOADER_INSTALL_DIR="${VULKAN_VENDOR_DIR}"/Vulkan-Loader/build                    \
        -DGLSLANG_INSTALL_DIR="${VENDOR_DIR}"/glslang/build/install                               \
        -DSPIRV_HEADERS_INSTALL_DIR="${VULKAN_VENDOR_DIR}"/SPIRV-Headers/build/install            \
        -DSPIRV_TOOLS_INSTALL_DIR="${VULKAN_VENDOR_DIR}"/SPIRV-Tools/build/install                \
        -DROBIN_HOOD_HASHING_INSTALL_DIR="${VULKAN_VENDOR_DIR}"/robin-hood-hashing/build/install  \
        -DCMAKE_BUILD_TYPE=Release                                                                \
        -DCMAKE_INSTALL_PREFIX="${BUILD_DIR}"                                                     \
        -DBUILD_TESTS=OFF                                                                         \
        -S"${VULKAN_VENDOR_DIR}"/Vulkan-ValidationLayers                                          \
        -B"${BUILD_DIR}"
    cmake --build "${BUILD_DIR}" --config Release --target install -j"${NUMBER_OF_PROCESSORS}"
    mkdir -p "${VULKAN_LIB_DIR}"/explicit_layer.d
    cp "${VULKAN_VENDOR_DIR}"/Vulkan-ValidationLayers/"${LOADER_INSTALL_DIR}"/**.json "${VULKAN_LIB_DIR}"/explicit_layer.d
    cp "${VULKAN_VENDOR_DIR}"/Vulkan-ValidationLayers/build/layers/**${LIB_SUFFIX} "${VULKAN_LIB_DIR}"/explicit_layer.d
}

setup_zlib() {
    echo "Cloning zLib..."
    update_submodules zlib

    mkdir -p "${VENDOR_DIR}"/zlib/build
    cd "${VENDOR_DIR}"/zlib; ./configure
    cd "${VENDOR_DIR}"/zlib; make install prefix="${VENDOR_DIR}"/zlib/build
    cd "${ROOT_DIR}"
}

setup_libpng() {
  echo "Cloning libPng..."
  update_submodules libpng

  mkdir -p "${VENDOR_DIR}"/libpng/build
  cd "${VENDOR_DIR}"/libpng; cmake . -DCMAKE_INSTALL_PREFIX="${VENDOR_DIR}"/libpng/build
  cd "${VENDOR_DIR}"/libpng; make
  cd "${VENDOR_DIR}"/libpng; make install
  cd "${ROOT_DIR}"
}

setup_freetype() {
  echo "Cloning FreeType..."

  update_submodules freetype
  local BUILD_DIR="${VULKAN_DIR}"/freetype
  mkdir -p "${BUILD_DIR}"
  cmake -G "${GENERATOR}" -DFT_DISABLE_BROTLI=TRUE -DFT_DISABLE_BZIP2=TRUE -DFT_DISABLE_HARFBUZZ=TRUE -B"${VENDOR_DIR}"/freetype/build -S"${VENDOR_DIR}"/freetype
  make -C "${VENDOR_DIR}"/freetype/build -j"${NUMBER_OF_PROCESSORS}"

  mkdir -p "${VENDOR_DIR}"/include/freetype
  cp -R "${VENDOR_DIR}"/freetype/include/** "${VENDOR_DIR}"/include/freetype
  cd "${ROOT_DIR}"
}

echo "OS detected: ${OS}"

echo "Setting up raylib..."
echo "Cloning raylib..."
git submodule update --init --recursive "${VENDOR_DIR}"/raylib-cpp

echo "Setting up utest..."
echo "Cloning utest..."
git submodule update --init --recursive "${VENDOR_DIR}"/utest.h

echo "Setting up GLFW..."
setup_glfw

echo "Setting up GLSLang..."
setup_glslang

echo "Setting up GLM..."
setup_glm

echo "Setting up Volk..."
setup_volk

echo "Setting up Vulkan Headers"
setup_vulkan_headers

cho "Setting up zlib"
setup_zlib

echo "Setting up libpng"
setup_libpng

echo "Setting up FreeType"
setup_freetype

echo "Setting up Vulkan Loader"
setup_vulkan_loader

if [[ "${OS}" == "macos" ]]; then
    echo "Setting up MoltenVk"
    setup_moltenVk
fi

if [[ $* == *--include-validation-layers* ]]; then
    echo "Setting up Robin Hood Hashing..."
    setup_robin_hood_hashing

    echo "Setting up SPIRV Headers..."
    setup_spirv_headers

    echo "Setting up SPIRV Tools..."
    setup_spirv_tools

    echo "Setting up Vulkan Validation Layers..."
    setup_validation_layers
fi

echo "Configuring environment file..."
{
    echo "DYLD_LIBRARY_PATH='${VULKAN_LIB_DIR}'"
    echo "VK_LAYER_PATH='${VULKAN_LIB_DIR}/explicit_layer.d'"
} > .env
if [[ "${OS}" == "macos" ]]; then
    echo "VK_ICD_FILENAMES='${VULKAN_LIB_DIR}/icd.d/MoltenVK_icd.json'" >> .env
fi

echo "Setup complete"
