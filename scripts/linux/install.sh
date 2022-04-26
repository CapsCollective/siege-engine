#!/bin/bash

ROOT_DIR="$(pwd)"
OS="Linux"
GENERATOR="Unix Makefiles"
NUMBER_OF_PROCESSORS="$(nproc)"

SUBMODULE_DIR="${ROOT_DIR}/vendor"
SUBMODULE_INCLUDE_DIR="${SUBMODULE_DIR}/include"
SUBMODULE_LIB_DIR="${ROOT_DIR}/lib/${OS}"

LOADER_INSTALL_DIR="${SUBMODULE_DIR}/Vulkan-Loader/build/loader"

echo "Installing vulkan dependencies..."

update_submodules() {
    git submodule update --init $1
}

mkdirp() { 
    mkdir -p $1 
}

setup_volk() {
    echo "Cloning Volk..."
    update_submodules vendor/volk

    mkdirp ${SUBMODULE_DIR}/include/volk

    cp ${SUBMODULE_DIR}/volk/volk.h ${SUBMODULE_INCLUDE_DIR}/volk 
    cp ${SUBMODULE_DIR}/volk/volk.c ${SUBMODULE_INCLUDE_DIR}/volk
}

setup_glfw() {
    echo "Cloning glfw..."

    update_submodules vendor/glfw

    cd ${SUBMODULE_DIR}/glfw ; cmake -G "${GENERATOR}" . ; make -j${NUMBER_OF_PROCESSORS}

    mkdirp ${SUBMODULE_LIB_DIR}

    cp ${SUBMODULE_DIR}/glfw/src/libglfw3.a ${SUBMODULE_LIB_DIR}

    cd ${ROOT_DIR}
}

setup_vulkan_headers() {
    update_submodules vendor/Vulkan-Headers

    cd ${SUBMODULE_DIR}/Vulkan-Headers ; git fetch --all --tags ; git checkout tags/v1.3.211
    
    mkdirp ${SUBMODULE_DIR}/Vulkan-Headers/build

    cd ${SUBMODULE_DIR}/Vulkan-Headers/build ; cmake -DCMAKE_INSTALL_PREFIX=install -G "${GENERATOR}" ..
    cd ${SUBMODULE_DIR}/Vulkan-Headers/build ; cmake --build . --target install

    mkdirp ${SUBMODULE_INCLUDE_DIR}/vulkan

    cp ${SUBMODULE_DIR}/Vulkan-Headers/include/vulkan/**.h ${SUBMODULE_INCLUDE_DIR}/vulkan

    cd ${ROOT_DIR}
}

setup_vulkan_loader() {
    echo "Cloning Vulkan Loader..."
    
    update_submodules vendor/Vulkan-Loader
    mkdirp ${SUBMODULE_DIR}/Vulkan-Loader/build
    
    cd ${SUBMODULE_DIR}/Vulkan-Loader/build ; cmake -DVULKAN_HEADERS_INSTALL_DIR=${SUBMODULE_DIR}/Vulkan-Headers/build/install ..
    cd ${SUBMODULE_DIR}/Vulkan-Loader/build ; cmake --build . --config Release

    mkdirp ${SUBMODULE_LIB_DIR}
    cp ${LOADER_INSTALL_DIR}/**.so ${SUBMODULE_LIB_DIR}

    cd ${ROOT_DIR}
}

setup_glslang() {
    update_submodules vendor/glslang

    mkdirp ${SUBMODULE_DIR}/glslang/build

    cd ${SUBMODULE_DIR}/glslang/build ; cmake -G "${GENERATOR}" -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=${SUBMODULE_DIR}/glslang/build/install ..
    cd ${SUBMODULE_DIR}/glslang/build ; cmake --build . --target install -- -j${NUMBER_OF_PROCESSORS}

    mkdirp ${SUBMODULE_LIB_DIR}
    cp ${SUBMODULE_DIR}/glslang/build/glslang/libglslang.a ${SUBMODULE_LIB_DIR}

    cd ${ROOT_DIR}
}

setup_spirv_headers() {
    update_submodules vendor/debug/SPIRV-Headers

    mkdirp ${SUBMODULE_DIR}/debug/SPIRV-Headers/build

    cd ${SUBMODULE_DIR}/debug/SPIRV-Headers/build ; cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=${SUBMODULE_DIR}/debug/SPIRV-Headers/build/install
    cd ${SUBMODULE_DIR}/debug/SPIRV-Headers/build ; cmake --build . --target install --config Release

    cd ${ROOT_DIR}
}

setup_spirv_tools() {
    update_submodules vendor/debug/SPIRV-Tools

    mkdirp ${SUBMODULE_DIR}/debug/SPIRV-Tools/build

    cd ${SUBMODULE_DIR}/debug/SPIRV-Tools/build ; python3 utils/git-sync-deps
    cd ${SUBMODULE_DIR}/debug/SPIRV-Tools/build ; cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=${SUBMODULE_DIR}/debug/SPIRV-Tools/build/install
    cd ${SUBMODULE_DIR}/debug/SPIRV-Tools/build ; cmake --build . --target install --config Release

    cd ${ROOT_DIR}
}

setup_robin_hood_hashing() {
    update_submodules vendor/debug/robin-hood-hashing

    cd ${SUBMODULE_DIR}/debug/robin-hood-hashing/build ; cmake -G "${GENERATOR}" ${SUBMODULE_DIR}/debug/robin-hood-hashing -DCMAKE_INSTALL_PREFIX=${SUBMODULE_DIR}/debug/robin-hood-hashing/build/install -DRH_STANDALONE_PROJECT=OFF -DCMAKE_BUILD_TYPE=Release
    cd ${SUBMODULE_DIR}/debug/robin-hood-hashing/build ; cmake --build ${SUBMODULE_DIR}/debug/robin-hood-hashing/build --target install -- -j${NUMBER_OF_PROCESSORS}

    cd ${ROOT_DIR}
}

setup_validation_layers() {
    update_submodules vendor/debug/Vulkan-ValidationLayers
    
    cd ${SUBMODULE_DIR}/debug/Vulkan-ValidationLayers ; git fetch --all --tags ; git checkout tags/v1.3.211

    mkdirp ${SUBMODULE_DIR}/debug/Vulkan-ValidationLayers/build

    cd ${SUBMODULE_DIR}/debug/Vulkan-ValidationLayers ; cmake \
        -DVULKAN_HEADERS_INSTALL_DIR=${SUBMODULE_DIR}/Vulkan-Headers/build/install \
        -DVULKAN_LOADER_INSTALL_DIR=${SUBMODULE_DIR}/Vulkan-Loader/build \
        -DGLSLANG_INSTALL_DIR=${SUBMODULE_DIR}/glslang/build/install \
        -DSPIRV_HEADERS_INSTALL_DIR=${SUBMODULE_DIR}/debug/SPIRV-Headers/build/install \
        -DSPIRV_TOOLS_INSTALL_DIR=${SUBMODULE_DIR}/debug/SPIRV-Tools/build/install \
        -DROBIN_HOOD_HASHING_INSTALL_DIR=${SUBMODULE_DIR}/debug/robin-hood-hashing/build/install \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_INSTALL_PREFIX=./ \
        -DBUILD_TESTS=OFF ..
    
    cd ${SUBMODULE_DIR}/debug/Vulkan-ValidationLayers/build ; cmake --build . --config Release --target install

    mkdirp ${SUBMODULE_LIB_DIR}/explicit_layer.d

    cp ${SUBMODULE_DIR}/debug/Vulkan-ValidationLayers/build/share/vulkan/explicit_layer.d/**.json ${SUBMODULE_LIB_DIR}/explicit_layer.d
    cp ${SUBMODULE_DIR}/debug/Vulkan-ValidationLayers/build/layers/**.so ${SUBMODULE_LIB_DIR}/explicit_layer.d
    cp ${SUBMODULE_LIB_DIR}/explicit_layer.d/**.so ${SUBMODULE_LIB_DIR}

    cd ${ROOT_DIR}
}

setup_vulkan_validation_layers() {
    setup_glslang
    setup_robin_hood_hashing
    setup_spirv_headers
    setup_spirv_tools
    setup_validation_layers
}

setup_volk
setup_glfw

if [ -z "${VULKAN_SDK}" ]
then
    echo "VULKAN_SDK has not been set. Setting up dependent repositories"
    setup_vulkan_headers
    setup_vulkan_loader

    if [ "$1" == "DEBUG" ]
    then
        echo "Setting up Validation Layers..."
        setup_vulkan_validation_layers
    fi
fi