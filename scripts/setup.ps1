# Copyright (c) 2020-present Caps Collective & contributors
# Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
#
# This code is released under an unmodified zlib license.
# For conditions of distribution and use, please see:
#     https://opensource.org/licenses/Zlib

param (
    [switch] $Include_Validation_Layers
)

[string] $Root_Dir=(pwd)
[string] $Vulkan_Version= "v1.3.250"
[string] $Spirv_Version="sdk-1.3.250.1"
[string] $Glslang_Version="12.2.0"
[string] $Robin_Hood_Hashing_Version="3.11.5"
[string] $Generator="MinGW Makefiles"
[string] $Vendor_Dir="$Root_Dir/vendor".Replace('\','/')

[string] $Bin_Dir="$Root_Dir/bin".Replace('\','/')
[string] $Build_Dir="examples/render/build"

[string] $Vulkan_Vendor_Dir="$Vendor_Dir/vulkan".Replace('\','/')
[string] $Vulkan_Lib_Dir="$Vulkan_Vendor_Dir/lib".Replace('\','/')
[string] $Vulkan_Include_Dir="$Vulkan_Vendor_Dir/include".Replace('\','/')
[string] $Volk_Include_Dir="$Vulkan_Vendor_Dir/include/volk".Replace('\','/')

[string] $CMAKE_COMPILERS="-DCMAKE_C_COMPILER=`"gcc.exe`" -DCMAKE_CXX_COMPILER=`"g++.exe`""

function Make-Dir {
    param ( [string] $Dir_Path )
    if (!(Test-Path $Dir_Path)) { New-Item -Path $Dir_Path -ItemType directory }
}

function Get-Files {
    param ([string] $Path=@(), [string[]] $Extensions=@())
    $items = Get-ChildItem -Path $Path/* -Include $Extensions -Recurse
    $items
    return
}

function Update-Submodule {
    param ([string] $Submodule)
    git submodule update --init --recursive --depth 1 $Vendor_Dir/$Submodule
}

function Checkout-Tags {
    param([string] $Path=@(), [string] $Tag=@())
    git -C "$Path"  fetch --all --tags ; git -C "$Path" checkout tags/"$Tag"
}

function Setup-Utest {
    Write-Output "Setting up utest..."
    Write-Output "Cloning utest..."
    Update-Submodule -Submodule utest.h
}

function Setup-Zlib {
    Write-Output "Setting up zlib..."
    Write-Output "Cloning zlib..."
    Update-Submodule zlib

    Write-Output "Building zlib..."
    [string] $build_dir = "$Vendor_Dir/zlib/build"

    Make-Dir $build_dir

    cmake                                   `
        -G "$Generator"                     `
        "$Vendor_Dir/zlib"                  `
        $CMAKE_COMPILER                     `
        -DCMAKE_INSTALL_PREFIX="$build_dir" `
        -S"$Vendor_Dir/zlib"                `
        -B"$build_dir"

    mingw32-make -C "$Build_Dir" install -j"$env:NUMBER_OF_PROCESSORS"

    if(!(Test-Path "$build_dir/lib/libz.a")) {Rename-Item -Path "$build_dir/lib/libzlibstatic.a" -NewName "libz.a"}
}

function Setup-LibPng {
    Write-Output "Setting up libpng..."
    Write-Output "Cloning libpng..."
    Update-Submodule libpng

    Write-Output "Building libpng..."
    [string] $build_dir = "$Vendor_Dir/libpng/build"
    [string] $zlib_dir = "$Vendor_Dir/zlib/build"

    Make-Dir $build_dir

    cmake                                       `
        -G "$Generator"                         `
        "$Vendor_Dir/libpng"                    `
        $CMAKE_COMPILER                         `
        -DCMAKE_INSTALL_PREFIX="$build_dir"     `
        -DPNG_BUILD_ZLIB=ON                     `
        -DZLIB_INCLUDE_DIRS="$zlib_dir/include" `
        -DZLIB_LIBRARIES="$zlib_dir/lib/libz.a" `
        -S"$Vendor_Dir/libpng"                  `
        -B"$build_dir"

    mingw32-make -C "$Build_Dir" install -j"$env:NUMBER_OF_PROCESSORS"
}

function Setup-FreeType {
    Write-Output "Setting up freetype..."
    Write-Output "Cloning freetype..."
    Update-Submodule freetype

    Write-Output "Building freetype..."
    [string] $build_dir = "$Vendor_Dir/freetype/build"
    [string] $zlib_build_dir = "$Vendor_Dir/zlib/build"
    [string] $libpng_build_dir = "$Vendor_Dir/libpng/build"

    Make-Dir $build_dir

    cmake                                                   `
        -G "$Generator"                                     `
        $CMAKE_COMPILER                                     `
        -DCMAKE_INSTALL_PREFIX="$build_dir"                 `
        -DFT_DISABLE_BROTLI=TRUE                            `
        -DFT_DISABLE_BZIP2=TRUE                             `
        -DFT_DISABLE_HARFBUZZ=TRUE                          `
        -DZLIB_LIBRARY="$zlib_build_dir/lib/libz.a"         `
        -DZLIB_INCLUDE_DIR="$zlib_build_dir/include"        `
        -DPNG_LIBRARY="$libpng_build_dir/lib/libpng.a"      `
        -DPNG_PNG_INCLUDE_DIR="$libpng_build_dir/include"   `
        -B"$build_dir"                                      `
        -S"$Vendor_Dir"/freetype

    mingw32-make -C "$Build_Dir" install -j"$env:NUMBER_OF_PROCESSORS"

    Make-Dir "$Vendor_Dir/include/freetype"

    Get-ChildItem "$Vendor_Dir/freetype/include" | Copy-Item -Destination "$Vendor_Dir/include/freetype" -Recurse
}

function Setup-Glfw {
    Write-Output "Setting up glfw..."
    Write-Output "Cloning glfw..."
    Update-Submodule glfw

    Write-Output "Building glfw..."
    [string] $build_dir = "$Vendor_Dir/glfw/build"

    cmake -G"$Generator" $CMAKE_COMPILER -DCMAKE_INSTALL_PREFIX="$build_dir" -B"$build_dir" -S"$Vendor_Dir/glfw"

    mingw32-make -C "$build_dir" -j"$env:NUMBER_OF_PROCESSORS"
}

function Setup-Glslang {
    Write-Output "Setting up glslang..."
    Write-Output "Cloning glslang..."
    Update-Submodule glslang
    Checkout-Tags "$Vendor_Dir/glslang" $Glslang_Version

    Write-Output "Building glslang..."
    [string] $build_dir = "$Vendor_Dir/glslang/build"

    Make-Dir $build_dir

    cmake                                   `
        -G "$Generator"                     `
        $CMAKE_COMPILER                     `
        -DBUILD_EXTERNAL=OFF                `
        -DCMAKE_INSTALL_PREFIX="$build_dir" `
        -DCMAKE_BUILD_TYPE=Release          `
        -S"$Vendor_Dir/glslang"             `
        -B"$build_dir"

    mingw32-make -C "$build_dir" install -j"$env:NUMBER_OF_PROCESSORS"
}

function Setup-Volk {
    Write-Output "Setting up volk..."
    Write-Output "Cloning volk..."
    Update-Submodule vulkan/volk

    Write-Output "Building volk..."
    Make-Dir $Volk_Include_Dir

    Copy-Item -Path "$Vulkan_Vendor_Dir/volk/volk.h" -Destination $Volk_Include_Dir
    Copy-Item -Path "$Vulkan_Vendor_Dir/volk/volk.c" -Destination $Volk_Include_Dir
}

function Setup-Vulkan-Headers {
    Write-Output "Setting up Vulkan Headers..."
    Write-Output "Cloning Vulkan Headers..."
    Update-Submodule vulkan/Vulkan-Headers
    Checkout-Tags "$Vulkan_Vendor_Dir/Vulkan-Headers" $Vulkan_Version

    Write-Output "Building Vulkan Headers..."
    [string] $build_dir = "$Vulkan_Vendor_Dir/Vulkan-Headers/build"

    Make-Dir $build_dir

    cmake                                               `
        -G "$Generator"                                 `
        $CMAKE_COMPILER                                 `
        -DCMAKE_INSTALL_PREFIX="$build_dir/install"     `
        -B"$build_dir"                                  `
        -S"$Vulkan_Vendor_Dir/Vulkan-Headers"

    mingw32-make -C $build_dir install -j"$env:NUMBER_OF_PROCESSORS"

    Make-Dir $Vulkan_Include_Dir

    Get-ChildItem "$Vulkan_Vendor_Dir/Vulkan-Headers/build/install/include" | Copy-Item -Destination $Vulkan_Include_Dir -Recurse
}

function Setup-Vulkan-Loader {
    Write-Output "Setting up Vulkan Loader..."
    Write-Output "Cloning Vulkan Loader..."
    Update-Submodule vulkan/Vulkan-Loader

    Checkout-Tags "$Vulkan_Vendor_Dir/Vulkan-Loader" $Vulkan_Version

    Write-Output "Building Vulkan Loader..."
    [string] $build_dir = "$Vulkan_Vendor_Dir/Vulkan-Loader/build"

    cmake                                                                               `
        $CMAKE_COMPILER                                                                 `
        -DCMAKE_INSTALL_PREFIX="$build_dir"                                             `
        -DVULKAN_HEADERS_INSTALL_DIR="$Vulkan_Vendor_Dir/Vulkan-Headers/build/install"  `
        -S"$Vulkan_Vendor_Dir/Vulkan-Loader"                                            `
        -B"$build_dir"

    cmake --build "$build_dir" --parallel $env:NUMBER_OF_PROCESSORS --config Release

    Make-Dir $Vulkan_Lib_Dir

    Copy-Item -Path "$build_dir/loader/Release/*" -Destination $Vulkan_Lib_Dir -Include "*.dll" -Recurse
}

function Setup-Robin-Hood-Hashing {
    Write-Output "Setting up Robin Hood Hashing..."
    Write-Output "Cloning Robin Hood Hashing..."
    Update-Submodule vulkan/robin-hood-hashing
    Checkout-Tags "$Vulkan_Vendor_Dir/robin-hood-hashing" $Robin_Hood_Hashing_Version

    Write-Output "Building Robin Hood Hashing..."
    [string] $build_dir = "$Vulkan_Vendor_Dir/robin-hood-hashing/build"

    cmake                                                       `
        -G "$Generator" "$Vulkan_Vendor_Dir"/robin-hood-hashing `
        $CMAKE_COMPILER                                         `
        -DCMAKE_INSTALL_PREFIX="$build_dir"/install             `
        -DRH_STANDALONE_PROJECT=OFF                             `
        -DCMAKE_BUILD_TYPE=Release                              `
        -S"$Vulkan_Vendor_Dir"/robin-hood-hashing               `
        -B"$build_dir"

    mingw32-make -C $build_dir install -j"$env:NUMBER_OF_PROCESSORS"
}

function Setup-Spirv-Headers {
    Write-Output "Setting up SPIRV Headers..."
    Write-Output "Cloning SPIRV Headers..."
    Update-Submodule vulkan/SPIRV-Headers

    Checkout-Tags "$Vulkan_Vendor_Dir/SPIRV-Headers" "$Spirv_Version"

    Write-Output "Building SPIRV Headers..."
    [string] $build_dir = "$Vulkan_Vendor_Dir/SPIRV-Headers/build"

    Make-Dir $build_dir

    cmake `
        $CMAKE_COMPILER                             `
        -DCMAKE_BUILD_TYPE=Release                  `
        -DCMAKE_INSTALL_PREFIX="$build_dir/install" `
        -S"$Vulkan_Vendor_Dir/SPIRV-Headers"        `
        -B"$build_dir"

    cmake --build "$build_dir" --target install --config Release --parallel $env:NUMBER_OF_PROCESSORS
}

function Setup-Spirv-Tools {
    Write-Output "Setting up Spirv Tools..."
    Write-Output "Cloning SPIRV Tools..."
    Update-Submodule vulkan/SPIRV-Tools

    Checkout-Tags "$Vulkan_Vendor_Dir/SPIRV-Tools" "$Spirv_Version"

    Write-Output "Building SPIRV Tools..."
    [string] $build_dir = "$Vulkan_Vendor_Dir/SPIRV-Tools/build"

    Make-Dir $build_dir

    cmake                                                               `
        ..                                                              `
        $CMAKE_COMPILER                                                 `
        -DCMAKE_BUILD_TYPE=Release                                      `
        -DSPIRV_SKIP_TESTS=ON                                           `
        -DSPIRV_WERROR=OFF                                              `
        -DSPIRV-Headers_SOURCE_DIR="$Vulkan_Vendor_Dir/SPIRV-Headers"   `
        -DCMAKE_INSTALL_PREFIX="$build_dir\install"                     `
        -S"$Vulkan_Vendor_Dir\SPIRV-Tools"                              `
        -B"$build_dir"

    cmake                                                               `
        $CMAKE_COMPILER                                                 `
        -DCMAKE_BUILD_TYPE=Release                                      `
        -DSPIRV_SKIP_TESTS=ON                                           `
        -DSPIRV_WERROR=OFF                                              `
        -DSPIRV-Headers_SOURCE_DIR="$Vulkan_Vendor_Dir/SPIRV-Headers"   `
        -DCMAKE_INSTALL_PREFIX="$build_dir\install"                     `
        -S"$Vulkan_Vendor_Dir\SPIRV-Tools"                              `
        -B"$build_dir"
    cmake --build $build_dir --parallel $env:NUMBER_OF_PROCESSORS --target install --config Release
}

function Setup-Validation-Layers {
    Write-Output "Setting up Vulkan Validation Layers..."
    Write-Output "Cloning Vulkan ValidationLayers..."
    Update-Submodule vulkan/Vulkan-ValidationLayers

    Checkout-Tags "$Vulkan_Vendor_Dir/Vulkan-ValidationLayers" "$Vulkan_Version"

    Write-Output "Building Vulkan Validation Layers..."
    [string] $build_dir = "$Vulkan_Vendor_Dir/Vulkan-ValidationLayers/build"

    Make-Dir $build_dir

    cmake                                                                                           `
        $CMAKE_COMPILER                                                                             `
        -DVULKAN_HEADERS_INSTALL_DIR="$Vulkan_Vendor_Dir/Vulkan-Headers/build/install"              `
        -DGLSLANG_INSTALL_DIR="$Vulkan_Vendor_Dir/glslang/build/install"                            `
        -DSPIRV_HEADERS_INSTALL_DIR="$Vulkan_Vendor_Dir/SPIRV-Headers/build/install"                `
        -DSPIRV_TOOLS_INSTALL_DIR="$Vulkan_Vendor_Dir/SPIRV-Tools/build/install"                    `
        -DROBIN_HOOD_HASHING_INSTALL_DIR="$Vulkan_Vendor_Dir/robin-hood-hashing/build/install"      `
        -DCMAKE_INSTALL_PREFIX="$build_dir"                                                         `
        -DBUILD_TESTS=OFF                                                                           `
        -S"$Vulkan_Vendor_Dir/Vulkan-ValidationLayers"                                              `
        -B"$build_dir"

    cmake --build $build_dir --parallel $env:NUMBER_OF_PROCESSORS --target install --config Release

    Make-Dir "$Vulkan_Lib_Dir/explicit_layer.d"

    Copy-Item -Path "$build_dir/bin/*" -Destination "$Vulkan_Lib_Dir/explicit_layer.d" -Include "*.json" -Recurse
    Copy-Item -Path "$build_dir/bin/*" -Destination "$Vulkan_Lib_Dir/explicit_layer.d" -Include "*.dll" -Recurse
}

Write-Output "Setting up dependencies..."

if (Test-Path "$Vulkan_Lib_Dir") { Remove-Item -Path "$Vulkan_Lib_Dir" -Recurse -Force }
if (Test-Path "$Vulkan_Vendor_Dir/vulkan/include") { Remove-Item -Path "$Vulkan_Vendor_Dir/vulkan/include" -Recurse -Force }
if (Test-Path "$Vendor_Dir/include") { Remove-Item -Path "$Vendor_Dir/include" -Recurse -Force }

Setup-Utest
Setup-Zlib
Setup-LibPng
Setup-FreeType
Setup-Glfw
Setup-Glslang
Setup-Volk
Setup-Vulkan-Headers
Setup-Vulkan-Loader

if ($Include_Validation_Layers)
{
    Setup-Robin-Hood-Hashing
    Setup-Spirv-Headers
    Setup-Spirv-Tools
    Setup-Validation-Layers

    Write-Output "Configuring environment file..."
    if (!(Test-Path "$Root_Dir/.env")) { New-Item -Path "$Root_Dir/.env" -ItemType file }
    Set-Content -Path "$Root_Dir/.env" -Value "VK_LAYER_PATH=$Bin_Dir\examples\render\build\lib\explicit_layer.d"
}

Write-Output "Siege setup complete"
