#!/bin/bash

ROOT_DIR="$(pwd)"
OS="macOS"
GENERATOR="Unix Makefiles"
NUMBER_OF_PROCESSORS="$(sysctl -n hw.ncpu)"

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

    cd ${SUBMODULE_DIR}/glfw ; cmake -G "${GENERATOR}" . ; make -j${NUMBER_OF_PROCESSORS}

    mkdirp ${SUBMODULE_LIB_DIR}

    cp ${SUBMODULE_DIR}/glfw/src/libglfw3.a ${SUBMODULE_LIB_DIR}

    cd ${ROOT_DIR}
}

setup_moltenVk() {
    echo "Cloning MoltenVk..."

    update_submodules MoltenVK
    cd ${SUBMODULE_DIR}/MoltenVK ; ./fetchDependencies --macos
    make macos -j${NUMBER_OF_PROCESSORS}

    mkdirp ${SUBMODULE_LIB_DIR}/icd.d
    mkdirp ${SUBMODULE_INCLUDE_DIR}/vulkan

    cp ${SUBMODULE_DIR}/MoltenVK/External/Vulkan-Headers/include/vulkan/**.h ${SUBMODULE_INCLUDE_DIR}/vulkan 
    cp ${SUBMODULE_DIR}/MoltenVK/Package/Latest/MoltenVK/dylib/macOS/** ${SUBMODULE_LIB_DIR}/icd.d 
    cp ${SUBMODULE_DIR}/MoltenVK/Package/Latest/MoltenVK/dylib/macOS/**.dylib ${SUBMODULE_LIB_DIR} 

    cd ${ROOT_DIR}
}

setup_volk
setup_glfw

if [ -z "${VULKAN_SDK}" ]
then
    echo "VULKAN SDK has not been set. Setting up MoltenVK..."
    setup_moltenVk
fi