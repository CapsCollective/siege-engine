#!/bin/bash
#
# Copyright (c) 2020-present Caps Collective & contributors
# Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
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
VULKAN_VERSION="vulkan-sdk-1.3.296.0"
SPIRV_VERSION="vulkan-sdk-1.3.296.0"
GLSLANG_VERSION="vulkan-sdk-1.3.296.0"
ROBIN_HOOD_HASHING_VERSION="3.11.5"
MOLTENVK_VERSION="v1.2.11"
FREETYPE_VERSION="VER-2-13-3"
LIBPNG_VERSION="v1.6.50"
ZLIB_VERSION="v1.3.1"
ASSIMP_VERSION="v6.0.2"
GENERATOR="Unix Makefiles"
VENDOR_DIR="${ROOT_DIR}/vendor"

# Vulkan dependency variables
VULKAN_VENDOR_DIR="${VENDOR_DIR}/vulkan"
VULKAN_LIB_DIR="${VULKAN_VENDOR_DIR}/lib"
VULKAN_INCLUDE_DIR="${VULKAN_VENDOR_DIR}/include"
VOLK_INCLUDE_DIR="${VULKAN_VENDOR_DIR}/include/volk"

update_submodules() {
    git submodule update --init --recursive --depth 1 "${VENDOR_DIR}"/"$1"
}

checkout_tags() {
    git -C "$1"  fetch --all --tags --force ; git -C "$1" checkout tags/"$2"
}

setup_utest() {
    echo "Setting up utest..."
    echo "Cloning utest..."
    update_submodules utest.h
}

setup_glfw() {
    echo "Setting up glfw..."
    echo "Cloning glfw..."
    update_submodules glfw

    local BUILD_DIR="${VENDOR_DIR}"/glfw/build

    echo "Building glfw..."
    cmake -G "${GENERATOR}" -DCMAKE_INSTALL_PREFIX="${BUILD_DIR}" -B"${BUILD_DIR}" -S"${VENDOR_DIR}"/glfw
    make -C "${BUILD_DIR}" -j"${NUMBER_OF_PROCESSORS}"
}

setup_glslang() {
    echo "Setting up glslang..."
    echo "Cloning glslang..."
    update_submodules glslang
    checkout_tags "${VENDOR_DIR}"/glslang "$GLSLANG_VERSION"

    echo "Building glslang..."
    mkdir -p "${VENDOR_DIR}"/glslang/build
    cmake \
        -DCMAKE_INSTALL_PREFIX="${VENDOR_DIR}"/glslang/build \
        -G "${GENERATOR}" -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_INSTALL_PREFIX="${VENDOR_DIR}"/glslang/build/install \
        -DALLOW_EXTERNAL_SPIRV_TOOLS=1 \
        -DSPIRV-Tools-opt_INCLUDE_DIRS="${VULKAN_VENDOR_DIR}"/SPIRV-Tools/build/install/include \
        -S"${VENDOR_DIR}"/glslang \
        -B"${VENDOR_DIR}"/glslang/build

    ln -sfn "${VULKAN_VENDOR_DIR}"/SPIRV-Tools "${VENDOR_DIR}"/glslang/External/spirv-tools

    cmake --build "${VENDOR_DIR}"/glslang/build --target install -- -j"${NUMBER_OF_PROCESSORS}"
}

setup_glm() {
  echo "Setting up glm..."
  echo "Cloning glm..."
  update_submodules glm
}

setup_volk() {
    echo "Setting up volk..."
    echo "Cloning volk..."
    update_submodules vulkan/volk

    echo "Building volk..."
    mkdir -p "${VOLK_INCLUDE_DIR}"
    cp "${VULKAN_VENDOR_DIR}"/volk/volk.h "${VOLK_INCLUDE_DIR}"
    cp "${VULKAN_VENDOR_DIR}"/volk/volk.c "${VOLK_INCLUDE_DIR}"
}

setup_vulkan_headers() {
    echo "Setting up Vulkan Headers..."
    echo "Cloning Vulkan Headers..."
    update_submodules vulkan/Vulkan-Headers
    checkout_tags "${VULKAN_VENDOR_DIR}"/Vulkan-Headers "$VULKAN_VERSION"

    echo "Building Vulkan Headers..."
    local BUILD_DIR="${VULKAN_VENDOR_DIR}"/Vulkan-Headers/build
    mkdir -p "${BUILD_DIR}"
    cmake -DCMAKE_INSTALL_PREFIX="${BUILD_DIR}"/install -G "${GENERATOR}" -B"${BUILD_DIR}" -S"${VULKAN_VENDOR_DIR}"/Vulkan-Headers
    cmake --build "${BUILD_DIR}" --target install
    mkdir -p "${VULKAN_INCLUDE_DIR}"
    cp -r "${VULKAN_VENDOR_DIR}"/Vulkan-Headers/build/install/include/** "${VULKAN_INCLUDE_DIR}"
}

setup_vulkan_loader() {
    echo "Setting up Vulkan Loader..."
    echo "Cloning Vulkan Loader..."
    update_submodules vulkan/Vulkan-Loader
    checkout_tags "${VULKAN_VENDOR_DIR}"/Vulkan-Loader "$VULKAN_VERSION"
    
    echo "Building Vulkan Loader..."
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
    echo "Setting up MoltenVk..."
    echo "Cloning MoltenVk..."
    update_submodules vulkan/MoltenVK
    checkout_tags "${VULKAN_VENDOR_DIR}"/MoltenVK ${MOLTENVK_VERSION}

    echo "Building MoltenVk..."
    (cd "${VULKAN_VENDOR_DIR}"/MoltenVK ; ./fetchDependencies --macos --v-headers-root "${VULKAN_VENDOR_DIR}"/Vulkan-Headers --glslang-root "${VENDOR_DIR}"/glslang)
    make -C "${VULKAN_VENDOR_DIR}"/MoltenVK macos -j"${NUMBER_OF_PROCESSORS}"
    mkdir -p "${VULKAN_LIB_DIR}"/icd.d
    mkdir -p "${VULKAN_INCLUDE_DIR}"
    cp "${VULKAN_VENDOR_DIR}"/MoltenVK/Package/Latest/MoltenVK/dylib/macOS/** "${VULKAN_LIB_DIR}"/icd.d
}

setup_robin_hood_hashing() {
    echo "Setting up Robin Hood Hashing..."
    echo "Cloning Robin Hood Hashing..."
    update_submodules vulkan/robin-hood-hashing
    checkout_tags "${VULKAN_VENDOR_DIR}"/robin-hood-hashing ${ROBIN_HOOD_HASHING_VERSION}

    echo "Building Robin Hood Hashing..."
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
    echo "Setting up SPIRV Headers..."
    echo "Cloning SPIRV Headers..."
    update_submodules vulkan/SPIRV-Headers

    checkout_tags "${VULKAN_VENDOR_DIR}"/SPIRV-Headers "${SPIRV_VERSION}"

    echo "Building SPIRV Headers..."
    local BUILD_DIR="${VULKAN_VENDOR_DIR}"/SPIRV-Headers/build
    mkdir -p "${BUILD_DIR}"
    cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="${BUILD_DIR}"/install -S"${VULKAN_VENDOR_DIR}"/SPIRV-Headers -B"${BUILD_DIR}"
    cmake --build "${BUILD_DIR}" --target install --config Release
}

setup_spirv_tools() {
    echo "Setting up SPIRV Tools..."
    echo "Cloning SPIRV Tools..."
    update_submodules vulkan/SPIRV-Tools
    checkout_tags "${VULKAN_VENDOR_DIR}"/SPIRV-Tools "$SPIRV_VERSION"

    echo "Building SPIRV Tools..."
    local BUILD_DIR="${VULKAN_VENDOR_DIR}"/SPIRV-Tools/build
    mkdir -p "${BUILD_DIR}"
    cmake -DCMAKE_BUILD_TYPE=Release -DSPIRV_SKIP_TESTS=ON -DSPIRV_WERROR=OFF -DSPIRV-Headers_SOURCE_DIR="${VULKAN_VENDOR_DIR}/SPIRV-Headers" -DCMAKE_INSTALL_PREFIX="${BUILD_DIR}"/install -S"${VULKAN_VENDOR_DIR}"/SPIRV-Tools -B"${BUILD_DIR}"
    cmake --build "${BUILD_DIR}" --target install --config Release -j"${NUMBER_OF_PROCESSORS}"

    ln -sfn "${VULKAN_VENDOR_DIR}"/SPIRV-Headers "${VULKAN_VENDOR_DIR}"/SPIRV-Tools/external/spirv-headers
}

setup_vulkan_utility_libraries() {
  echo "Setting up Vulkan Utility Libraries..."
  echo "Cloning Vulkan Utility Libraries..."

  update_submodules vulkan/Vulkan-Utility-Libraries
  checkout_tags "${VULKAN_VENDOR_DIR}"/Vulkan-Utility-Libraries "$VULKAN_VERSION"

  echo "Building Vulkan Utility Libraries..."
  local BUILD_DIR="${VULKAN_VENDOR_DIR}"/Vulkan-Utility-Libraries/build
  mkdir -p "${BUILD_DIR}"

  cmake -S "${VULKAN_VENDOR_DIR}"/Vulkan-Utility-Libraries -B "${BUILD_DIR}" -DCMAKE_BUILD_TYPE=Release -DUPDATE_DEPS=OFF -DVULKAN_HEADERS_INSTALL_DIR="${VULKAN_VENDOR_DIR}"/Vulkan-Headers/build/install -DCMAKE_INSTALL_PREFIX="${BUILD_DIR}"/install
  cmake --build "${BUILD_DIR}" --target install --config Release -j"${NUMBER_OF_PROCESSORS}"
}

setup_validation_layers() {
    echo "Setting up Vulkan Validation Layers..."
    echo "Cloning Vulkan Validation Layers..."
    update_submodules vulkan/Vulkan-ValidationLayers
    checkout_tags "${VULKAN_VENDOR_DIR}"/Vulkan-ValidationLayers "$VULKAN_VERSION"

    echo "Building Vulkan Validation Layers..."
    local BUILD_DIR="${VULKAN_VENDOR_DIR}"/Vulkan-ValidationLayers/build
    mkdir -p "${BUILD_DIR}"
    cmake \
        -DVULKAN_HEADERS_INSTALL_DIR="${VULKAN_VENDOR_DIR}"/Vulkan-Headers/build/install                        \
        -DGLSLANG_INSTALL_DIR="${VENDOR_DIR}"/glslang/build/install                                             \
        -DSPIRV_HEADERS_INSTALL_DIR="${VULKAN_VENDOR_DIR}"/SPIRV-Headers/build/install                          \
        -DSPIRV_TOOLS_INSTALL_DIR="${VULKAN_VENDOR_DIR}"/SPIRV-Tools/build/install                              \
        -DROBIN_HOOD_HASHING_INSTALL_DIR="${VULKAN_VENDOR_DIR}"/robin-hood-hashing/build/install                \
        -DVULKAN_UTILITY_LIBRARIES_INSTALL_DIR="${VULKAN_VENDOR_DIR}"/Vulkan-Utility-Libraries/build/install    \
        -DCMAKE_BUILD_TYPE=Release                                                                              \
        -DCMAKE_INSTALL_PREFIX="${BUILD_DIR}"                                                                   \
        -DBUILD_TESTS=OFF                                                                                       \
        -S"${VULKAN_VENDOR_DIR}"/Vulkan-ValidationLayers                                                        \
        -B"${BUILD_DIR}"
    cmake --build "${BUILD_DIR}" --config Release --target install -j"${NUMBER_OF_PROCESSORS}"
    mkdir -p "${VULKAN_LIB_DIR}"/explicit_layer.d
    cp "${VULKAN_VENDOR_DIR}"/Vulkan-ValidationLayers/"${LOADER_INSTALL_DIR}"/**.json "${VULKAN_LIB_DIR}"/explicit_layer.d
    cp "${VULKAN_VENDOR_DIR}"/Vulkan-ValidationLayers/build/layers/**${LIB_SUFFIX} "${VULKAN_LIB_DIR}"/explicit_layer.d
}

setup_zlib() {
    echo "Setting up zlib..."
    echo "Cloning zlib..."
    update_submodules zlib
    checkout_tags "${VENDOR_DIR}"/zlib ${ZLIB_VERSION}

    echo "Building zlib..."
    mkdir -p "${VENDOR_DIR}"/zlib/build
    local BUILD_DIR="${VENDOR_DIR}"/zlib/build
    (cd "${VENDOR_DIR}"/zlib && ./configure --prefix="${VENDOR_DIR}"/zlib/build --static)
    make -f "${VENDOR_DIR}"/zlib/Makefile -C "${VENDOR_DIR}"/zlib  install prefix="${VENDOR_DIR}"/zlib/build -j"${NUMBER_OF_PROCESSORS}"
}

setup_libpng() {
  echo "Setting up libpng..."
  echo "Cloning libpng..."
  update_submodules libpng
  checkout_tags "${VENDOR_DIR}"/libpng ${LIBPNG_VERSION}

  echo "Building libpng..."
  local BUILD_DIR="${VENDOR_DIR}"/libpng/build
  local ZLIB_BUILD_DIR="${VENDOR_DIR}"/zlib/build
  mkdir -p "${BUILD_DIR}"
  cmake "${VENDOR_DIR}"/libpng \
      -DCMAKE_INSTALL_PREFIX="${BUILD_DIR}" \
      -DZLIB_INCLUDE_DIRS="${ZLIB_BUILD_DIR}"/include \
      -DZLIB_LIBRARIES="${ZLIB_BUILD_DIR}"/lib/libz.a \
      -S "${VENDOR_DIR}"/libpng \
      -B"${BUILD_DIR}"
  make -C "${VENDOR_DIR}"/libpng/build install -j"${NUMBER_OF_PROCESSORS}"
}

setup_freetype() {
  echo "Setting up freetype..."
  echo "Cloning freetype..."
  update_submodules freetype
  checkout_tags "${VENDOR_DIR}"/freetype ${FREETYPE_VERSION}

  echo "Building freetype..."
  local BUILD_DIR="${VENDOR_DIR}"/freetype/build
  local ZLIB_BUILD_DIR="${VENDOR_DIR}"/zlib/build
  local LIBPNG_BUILD_DIR="${VENDOR_DIR}"/libpng/build
  mkdir -p "${BUILD_DIR}"
  cmake -G "${GENERATOR}" \
      -DCMAKE_INSTALL_PREFIX="${BUILD_DIR}" \
      -DFT_DISABLE_BROTLI=TRUE \
      -DFT_DISABLE_BZIP2=TRUE \
      -DFT_DISABLE_HARFBUZZ=TRUE \
      -DZLIB_LIBRARY="${ZLIB_BUILD_DIR}"/lib/libz.a \
      -DZLIB_INCLUDE_DIR="${ZLIB_BUILD_DIR}"/include \
      -DPNG_LIBRARY="${LIBPNG_BUILD_DIR}"/lib/libpng.a \
      -DPNG_PNG_INCLUDE_DIR="${LIBPNG_BUILD_DIR}"/include \
      -B"${BUILD_DIR}" \
      -S"${VENDOR_DIR}"/freetype
  make -C "${BUILD_DIR}" -j"${NUMBER_OF_PROCESSORS}"

  mkdir -p "${VENDOR_DIR}"/include/freetype
  cp -R "${VENDOR_DIR}"/freetype/include/** "${VENDOR_DIR}"/include/freetype
}

setup_assimp() {
    echo "Setting up assimp..."
    echo "Cloning assimp..."
    update_submodules assimp
    checkout_tags "${VENDOR_DIR}"/assimp ${ASSIMP_VERSION}

    echo "Building assimp..."
    mkdir -p "${VENDOR_DIR}"/assimp/build
    cmake "${VENDOR_DIR}"/assimp \
        -DCMAKE_INSTALL_PREFIX="${VENDOR_DIR}"/assimp/build \
        -DBUILD_SHARED_LIBS=OFF \
        -DZLIB_FOUND=1 \
        -DZLIB_LIBRARIES="${ZLIB_BUILD_DIR}"/lib/libz.a \
        -DZLIB_INCLUDE_DIR="${ZLIB_BUILD_DIR}"/include \
        -DASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT=OFF \
        -DASSIMP_BUILD_TESTS=OFF \
        -DASSIMP_INSTALL=OFF \
        -DASSIMP_BUILD_ZLIB=OFF \
        -S "${VENDOR_DIR}"/assimp \
        -B"${VENDOR_DIR}"/assimp/build
    make -C "${VENDOR_DIR}"/assimp/build -j"${NUMBER_OF_PROCESSORS}"
}

echo "OS detected: ${OS}"
echo "Setting up dependencies..."

setup_utest
setup_glfw
setup_spirv_headers
setup_spirv_tools
setup_glslang
setup_glm
setup_volk
setup_vulkan_headers
setup_zlib
setup_libpng
setup_freetype
setup_assimp
setup_vulkan_loader

if [[ "${OS}" == "macos" ]]; then
    setup_moltenVk
fi

if [[ $* == *--include-validation-layers* ]]; then
    setup_robin_hood_hashing
    setup_vulkan_utility_libraries
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

echo "Siege setup complete"
