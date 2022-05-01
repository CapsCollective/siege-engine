#!/bin/bash

ROOT_DIR="$(pwd)"

if [[ "$OSTYPE" == "darwin"* ]]; then
    OS="macOS"
    NUMBER_OF_PROCESSORS="$(sysctl -n hw.ncpu)"
fi

if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    OS="Linux"
    NUMBER_OF_PROCESSORS="$(nproc)"
fi

GENERATOR="Unix Makefiles"

SUBMODULE_DIR="${ROOT_DIR}/vendor"
SUBMODULE_INCLUDE_DIR="${SUBMODULE_DIR}/include"
SUBMODULE_LIB_DIR="${ROOT_DIR}/lib/${OS}"

echo "Installing vulkan dependencies..."

update_submodules() {
    git submodule update --init ${SUBMODULE_DIR}/$1
}

mkdirp() { 
    mkdir -p $1 
}

setup_volk() {
    echo "Cloning Volk..."
    update_submodules volk

    mkdirp ${SUBMODULE_DIR}/include/volk

    cp ${SUBMODULE_DIR}/volk/volk.h ${SUBMODULE_INCLUDE_DIR}/volk 
    cp ${SUBMODULE_DIR}/volk/volk.c ${SUBMODULE_INCLUDE_DIR}/volk
}

setup_glfw() {
    echo "Cloning glfw..."

    update_submodules glfw

    cmake -G "${GENERATOR}" -B${SUBMODULE_DIR}/glfw -S${SUBMODULE_DIR}/glfw
    make -C ${SUBMODULE_DIR}/glfw -j${NUMBER_OF_PROCESSORS}

    mkdirp ${SUBMODULE_LIB_DIR}

    cp ${SUBMODULE_DIR}/glfw/src/libglfw3.a ${SUBMODULE_LIB_DIR}
}

setup_moltenVk() {
    echo "Cloning MoltenVk..."

    update_submodules MoltenVK

    (cd ${SUBMODULE_DIR}/MoltenVK ; ./fetchDependencies --macos --v-headers-root ${SUBMODULE_DIR}/Vulkan-Headers)
    make -C ${SUBMODULE_DIR}/MoltenVK macos -j${NUMBER_OF_PROCESSORS}

    mkdirp ${SUBMODULE_LIB_DIR}/icd.d
    mkdirp ${SUBMODULE_INCLUDE_DIR}/vulkan

    cp ${SUBMODULE_DIR}/MoltenVK/Package/Latest/MoltenVK/dylib/macOS/** ${SUBMODULE_LIB_DIR}/icd.d 
    cp ${SUBMODULE_DIR}/MoltenVK/Package/Latest/MoltenVK/dylib/macOS/**.dylib ${SUBMODULE_LIB_DIR} 
}

setup_vulkan_headers() {
    update_submodules Vulkan-Headers

    git -C ${SUBMODULE_DIR}/Vulkan-Headers fetch --all --tags ; git -C ${SUBMODULE_DIR}/Vulkan-Headers checkout tags/v1.3.211
    
    mkdirp ${SUBMODULE_DIR}/Vulkan-Headers/build

    cmake -DCMAKE_INSTALL_PREFIX=${SUBMODULE_DIR}/Vulkan-Headers/build/install -G "${GENERATOR}" -B${SUBMODULE_DIR}/Vulkan-Headers/build -S${SUBMODULE_DIR}/Vulkan-Headers
    cmake --build ${SUBMODULE_DIR}/Vulkan-Headers/build --target install 

    mkdirp ${SUBMODULE_INCLUDE_DIR}/vulkan

    cp ${SUBMODULE_DIR}/Vulkan-Headers/include/vulkan/**.h ${SUBMODULE_INCLUDE_DIR}/vulkan
}

setup_vulkan_loader() {
    echo "Cloning Vulkan Loader..."
    
    update_submodules Vulkan-Loader
    mkdirp ${SUBMODULE_DIR}/Vulkan-Loader/build
    
    cmake -DCMAKE_INSTALL_PREFIX=${SUBMODULE_DIR}/Vulkan-Loader/build -DVULKAN_HEADERS_INSTALL_DIR=${SUBMODULE_DIR}/Vulkan-Headers/build/install -S${SUBMODULE_DIR}/Vulkan-Loader -B${SUBMODULE_DIR}/Vulkan-Loader/build
    cmake --build ${SUBMODULE_DIR}/Vulkan-Loader/build --config Release

    mkdirp ${SUBMODULE_LIB_DIR}
    cp ${SUBMODULE_DIR}/Vulkan-Loader/build/loader/**.so ${SUBMODULE_LIB_DIR}
}

setup_glslang() {
    update_submodules glslang

    mkdirp ${SUBMODULE_DIR}/glslang/build

    cmake -DCMAKE_INSTALL_PREFIX=${SUBMODULE_DIR}/glslang/build -G "${GENERATOR}" -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=${SUBMODULE_DIR}/glslang/build/install -S${SUBMODULE_DIR}/glslang -B${SUBMODULE_DIR}/glslang/build
    cmake --build ${SUBMODULE_DIR}/glslang/build --target install -- -j${NUMBER_OF_PROCESSORS}

    mkdirp ${SUBMODULE_LIB_DIR}
    cp ${SUBMODULE_DIR}/glslang/build/glslang/libglslang.a ${SUBMODULE_LIB_DIR}
}

setup_spirv_headers() {
    update_submodules debug/SPIRV-Headers

    mkdirp ${SUBMODULE_DIR}/debug/SPIRV-Headers/build

    cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=${SUBMODULE_DIR}/debug/SPIRV-Headers/build/install -S${SUBMODULE_DIR}/debug/SPIRV-Headers -B${SUBMODULE_DIR}/debug/SPIRV-Headers/build
    cmake --build ${SUBMODULE_DIR}/debug/SPIRV-Headers/build --target install --config Release
}

setup_spirv_tools() {
    update_submodules debug/SPIRV-Tools

    mkdirp ${SUBMODULE_DIR}/debug/SPIRV-Tools/build

    python3 ${SUBMODULE_DIR}/debug/SPIRV-Tools/utils/git-sync-deps
    cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=${SUBMODULE_DIR}/debug/SPIRV-Tools/build/install -S${SUBMODULE_DIR}/debug/SPIRV-Tools -B${SUBMODULE_DIR}/debug/SPIRV-Tools/build
    cmake --build ${SUBMODULE_DIR}/debug/SPIRV-Tools/build --target install --config Release
}

setup_robin_hood_hashing() {
    update_submodules debug/robin-hood-hashing

    cmake -G "${GENERATOR}" ${SUBMODULE_DIR}/debug/robin-hood-hashing -DCMAKE_INSTALL_PREFIX=${SUBMODULE_DIR}/debug/robin-hood-hashing/build/install -DRH_STANDALONE_PROJECT=OFF -DCMAKE_BUILD_TYPE=Release -S${SUBMODULE_DIR}/debug/robin-hood-hashing -B${SUBMODULE_DIR}/debug/robin-hood-hashing/build
    cmake --build ${SUBMODULE_DIR}/debug/robin-hood-hashing/build --target install -- -j${NUMBER_OF_PROCESSORS}
}

setup_validation_layers() {
    update_submodules debug/Vulkan-ValidationLayers
    
    git -C ${SUBMODULE_DIR}/debug/Vulkan-ValidationLayers  fetch --all --tags ; git -C ${SUBMODULE_DIR}/debug/Vulkan-ValidationLayers checkout tags/v1.3.211

    mkdirp ${SUBMODULE_DIR}/debug/Vulkan-ValidationLayers/build

    cmake \
    -DVULKAN_HEADERS_INSTALL_DIR=${SUBMODULE_DIR}/Vulkan-Headers/build/install \
    -DVULKAN_LOADER_INSTALL_DIR=${SUBMODULE_DIR}/Vulkan-Loader/build \
    -DGLSLANG_INSTALL_DIR=${SUBMODULE_DIR}/glslang/build/install \
    -DSPIRV_HEADERS_INSTALL_DIR=${SUBMODULE_DIR}/debug/SPIRV-Headers/build/install \
    -DSPIRV_TOOLS_INSTALL_DIR=${SUBMODULE_DIR}/debug/SPIRV-Tools/build/install \
    -DROBIN_HOOD_HASHING_INSTALL_DIR=${SUBMODULE_DIR}/debug/robin-hood-hashing/build/install \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX=./ \
    -DBUILD_TESTS=OFF \
    -S${SUBMODULE_DIR}/debug/Vulkan-ValidationLayers
    -B${SUBMODULE_DIR}/debug/Vulkan-ValidationLayers/build
    
    cmake --build ${SUBMODULE_DIR}/debug/Vulkan-ValidationLayers/build --config Release --target install

    mkdirp ${SUBMODULE_LIB_DIR}/explicit_layer.d

    cp ${SUBMODULE_DIR}/debug/Vulkan-ValidationLayers/build/share/vulkan/explicit_layer.d/**.json ${SUBMODULE_LIB_DIR}/explicit_layer.d
    cp ${SUBMODULE_DIR}/debug/Vulkan-ValidationLayers/build/layers/**.so ${SUBMODULE_LIB_DIR}/explicit_layer.d
    cp ${SUBMODULE_LIB_DIR}/explicit_layer.d/**.so ${SUBMODULE_LIB_DIR}
}

setup_vulkan_validation_layers() {
    setup_robin_hood_hashing
    setup_spirv_headers
    setup_spirv_tools
    setup_validation_layers
}

setup_volk
setup_glfw

if [[ -z "${VULKAN_SDK}" ]]; then
    
    echo "OS detected: ${OS}"
    echo "# Environment variables for Vulkan." > .env

    setup_vulkan_headers

    echo "VULKAN_INCLUDE_DIR=${SUBMODULE_INCLUDE_DIR}/vulkan" >> .env

    setup_glslang

    echo "GLSLC=${SUBMODULE_DIR}/glslang/build/install/bin/glslangValidator" >> .env

    if [[ "${OS}" == "macOS" ]]; then 
        
        echo "VULKAN SDK has not been set. Setting up MoltenVK..."

        setup_moltenVk

        echo "MoltenVK successfully installed! Generating .env file..."

        echo "VK_ICD_FILENAMES=${SUBMODULE_LIB_DIR}/icd.d/MoltenVK_icd.json" >> .env
    fi 

    if [[ "$OSTYPE" == "linux-gnu"* ]]; then

        setup_vulkan_loader

        if [[ "$1" == "DEBUG" ]]; then
            
            echo "Setting up Validation Layers..."

            setup_vulkan_validation_layers

            echo "VK_LAYER_PATH=${SUBMODULE_LIB_DIR}/explicit_layer.d" >> .env
        fi
    fi

    echo "DYLD_LIBRARY_PATH=${SUBMODULE_LIB_DIR}" >> .env
else 
    echo "VULKAN_SDK has been detected. Will use SDK resources for build"
fi