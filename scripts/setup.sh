#!/bin/bash

# Find repo root
SCRIPT_DIR=$(dirname "${BASH_SOURCE[0]}")
cd "$SCRIPT_DIR/.." || exit
ROOT_DIR=$(pwd -P)
BIN_DIR="$ROOT_DIR/bin"

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
VULKAN_VERSION="1.3.211"
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
    git -C "$1"  fetch --all --tags ; git -C "$1" checkout tags/v"$2"
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

    echo "Setting up SPIRV Headers..."
    local BUILD_DIR="${VULKAN_VENDOR_DIR}"/SPIRV-Headers/build
    mkdir -p "${BUILD_DIR}"
    cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="${BUILD_DIR}"/install -S"${VULKAN_VENDOR_DIR}"/SPIRV-Headers -B"${BUILD_DIR}"
    cmake --build "${BUILD_DIR}" --target install --config Release
}

setup_spirv_tools() {
    echo "Cloning SPIRV Tools..."
    update_submodules vulkan/SPIRV-Tools

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
        -DVULKAN_HEADERS_INSTALL_DIR="${VULKAN_VENDOR_DIR}"/Vulkan-Headers/build/install \
        -DVULKAN_LOADER_INSTALL_DIR="${VULKAN_VENDOR_DIR}"/Vulkan-Loader/build \
        -DGLSLANG_INSTALL_DIR="${VENDOR_DIR}"/glslang/build/install \
        -DSPIRV_HEADERS_INSTALL_DIR="${VULKAN_VENDOR_DIR}"/SPIRV-Headers/build/install \
        -DSPIRV_TOOLS_INSTALL_DIR="${VULKAN_VENDOR_DIR}"/SPIRV-Tools/build/install \
        -DROBIN_HOOD_HASHING_INSTALL_DIR="${VULKAN_VENDOR_DIR}"/robin-hood-hashing/build/install \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_INSTALL_PREFIX="${BUILD_DIR}" \
        -DBUILD_TESTS=OFF \
        -S"${VULKAN_VENDOR_DIR}"/Vulkan-ValidationLayers \
        -B"${BUILD_DIR}"
    cmake --build "${BUILD_DIR}" --config Release --target install -j"${NUMBER_OF_PROCESSORS}"
    mkdir -p "${VULKAN_LIB_DIR}"/explicit_layer.d
    cp "${VULKAN_VENDOR_DIR}"/Vulkan-ValidationLayers/"${LOADER_INSTALL_DIR}"/**.json "${VULKAN_LIB_DIR}"/explicit_layer.d
    cp "${VULKAN_VENDOR_DIR}"/Vulkan-ValidationLayers/build/layers/**${LIB_SUFFIX} "${VULKAN_LIB_DIR}"/explicit_layer.d
}

echo "OS detected: ${OS}"

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
    echo "DYLD_LIBRARY_PATH='${BIN_DIR}/lib'"
    echo "VK_LAYER_PATH='${BIN_DIR}/lib/explicit_layer.d'"
} > .env
if [[ "${OS}" == "macos" ]]; then
    echo "VK_ICD_FILENAMES='${BIN_DIR}/lib/icd.d/MoltenVK_icd.json'" >> .env
fi

echo "Setup complete"