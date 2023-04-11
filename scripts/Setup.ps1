param (
    [switch] $Include_Validation_Layers
)

[string] $Root_Dir=(pwd)
[string] $Vulkan_Version= "v1.3.211"
[string] $Spirv_Version="v2022.4"
[string] $Generator="MinGW Makefiles"
[string] $Vendor_Dir="$Root_Dir\vendor"

[string] $Bin_Dir="$Root_Dir\bin"
[string] $Build_Dir="examples\render\build"

[string] $Vulkan_Vendor_Dir="$Vendor_Dir/vulkan"
[string] $Vulkan_Lib_Dir="$Vulkan_Vendor_Dir/lib"
[string] $Vulkan_Include_Dir="$Vulkan_Vendor_Dir/include/vulkan"
[string] $Volk_Include_Dir="$Vulkan_Vendor_Dir/include/volk"

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
    git submodule update --init --recursive $Vendor_Dir/$Submodule
}

function Checkout-Tags {
    param([string] $Path=@(), [string] $Tag=@())
    git -C "$Path"  fetch --all --tags ; git -C "$Path" checkout tags/"$Tag"
}

function Setup-Zlib {
    Update-Submodule zlib

    [string] $build_dir = "$Vendor_Dir/zlib/build"

    Make-Dir $build_dir

    cmake -G "$Generator" "$Vendor_Dir/zlib" -DCMAKE_INSTALL_PREFIX="$build_dir" -S"$Vendor_Dir/zlib" -B"$build_dir"

    make -C "$Build_Dir" install -j"$env:NUMBER_OF_PROCESSORS"

    if(!(Test-Path "$build_dir/lib/libz.a")) {Rename-Item -Path "$build_dir/lib/libzlibstatic.a" -NewName "libz.a"}
}

function Setup-LibPng {
    Update-Submodule libpng

    [string] $build_dir = "$Vendor_Dir/libpng/build"

    Make-Dir $build_dir

    cmake -G "$Generator" "$Vendor_Dir/libpng" -DCMAKE_INSTALL_PREFIX="$build_dir" -S"$Vendor_Dir/libpng" -B"$build_dir"

    make -C "$Build_Dir" install -j"$env:NUMBER_OF_PROCESSORS"
}

function Setup-FreeType {
    Update-Submodule freetype

    [string] $build_dir = "$Vendor_Dir/freetype/build"
    [string] $zlib_build_dir = "$Vendor_Dir/zlib/build"
    [string] $libpng_build_dir = "$Vendor_Dir/libpng/build"

    Make-Dir $build_dir

    cmake `
        -G "$Generator" `
        -DCMAKE_INSTALL_PREFIX="$build_dir" `
        -DFT_DISABLE_BROTLI=TRUE `
        -DFT_DISABLE_BZIP2=TRUE `
        -DFT_DISABLE_HARFBUZZ=TRUE `
        -DZLIB_LIBRARY="$zlib_build_dir/lib/libz.a" `
        -DZLIB_INCLUDE_DIR="$zlib_build_dir" `
        -DPNG_LIBRARY="$libpng_build_dir/lib/libpng.a" `
        -DPNG_PNG_INCLUDE_DIR="$libpng_build_dir/include" `
        -B"$build_dir" `
        -S"$Vendor_Dir"/freetype

    make -C "$Build_Dir" install -j"$env:NUMBER_OF_PROCESSORS"

    Make-Dir "$Vendor_Dir/include/freetype"

    Get-ChildItem "$Vendor_Dir/freetype/include" | Copy-Item -Destination "$Vendor_Dir/include/freetype" -Recurse
}

function Setup-Glfw {
    Update-Submodule glfw

    [string] $build_dir = "$Vendor_Dir/glfw/build"

    cmake -G"$Generator" -DCMAKE_INSTALL_PREFIX="$build_dir" -B"$build_dir" -S"$Vendor_Dir/glfw"

    make -C "$build_dir" -j"$env:NUMBER_OF_PROCESSORS"
}

function Setup-Glslang {
    Update-Submodule glslang

    [string] $build_dir = "$Vendor_Dir/glslang/build"

    Make-Dir $build_dir

    cmake `
        -G "$Generator" `
        -DCMAKE_INSTALL_PREFIX="$build_dir" `
        -DCMAKE_BUILD_TYPE=Release `
        -S"$Vendor_Dir/glslang" `
        -B"$build_dir"

    make -C "$build_dir" install -j"$env:NUMBER_OF_PROCESSORS"
}

function Setup-Volk {
    Update-Submodule vulkan/volk

    Make-Dir $Volk_Include_Dir

    Copy-Item -Path "$Vulkan_Vendor_Dir/volk/volk.h" -Destination $Volk_Include_Dir
    Copy-Item -Path "$Vulkan_Vendor_Dir/volk/volk.c" -Destination $Volk_Include_Dir
}

function Setup-Vulkan-Headers {
    Update-Submodule vulkan/Vulkan-Headers
    Checkout-Tags "$Vulkan_Vendor_Dir/Vulkan-Headers" $Vulkan_Version

    [string] $build_dir = "$Vulkan_Vendor_Dir/Vulkan-Headers/build"

    Make-Dir $build_dir

    cmake -G "$Generator" -DCMAKE_INSTALL_PREFIX="$build_dir/install"  -B"$build_dir" -S"$Vulkan_Vendor_Dir/Vulkan-Headers"
    make -C $build_dir install -j"$env:NUMBER_OF_PROCESSORS"

    Make-Dir $Vulkan_Include_Dir

    Copy-Item -Path "$Vulkan_Vendor_Dir/Vulkan-Headers/include/vulkan/*" -Destination $Vulkan_Include_Dir -Include "*.h"
}

function Setup-Vulkan_Loader {
    Update-Submodule vulkan/Vulkan-Loader

    Checkout-Tags "$Vulkan_Vendor_Dir/Vulkan-Loader" $Vulkan_Version

    [string] $build_dir = "$Vulkan_Vendor_Dir/Vulkan-Loader/build"

    cmake `
        -DCMAKE_INSTALL_PREFIX="$build_dir" `
        -DVULKAN_HEADERS_INSTALL_DIR="$Vulkan_Vendor_Dir/Vulkan-Headers/build/install" `
        -S"$Vulkan_Vendor_Dir/Vulkan-Loader" `
        -B"$build_dir"

    cmake --build "$build_dir" --parallel $env:NUMBER_OF_PROCESSORS --config Release

    Make-Dir $Vulkan_Lib_Dir

    Copy-Item -Path "$build_dir/loader/Release/*" -Destination $Vulkan_Lib_Dir -Include "*.dll" -Recurse
}

function Setup-Robin-Hood-Hashing
{
    Update-Submodule vulkan/robin-hood-hashing

    [string] $build_dir = "$Vulkan_Vendor_Dir/robin-hood-hashing/build"

    cmake `
        -G "$Generator" "$Vulkan_Vendor_Dir"/robin-hood-hashing `
        -DCMAKE_INSTALL_PREFIX="$build_dir"/install `
        -DRH_STANDALONE_PROJECT=OFF `
        -DCMAKE_BUILD_TYPE=Release `
        -S"$Vulkan_Vendor_Dir"/robin-hood-hashing `
        -B"$build_dir"

    make -C $build_dir install -j"$env:NUMBER_OF_PROCESSORS"
}

function Setup-Spirv-Headers {
    Update-Submodule vulkan/SPIRV-Headers

    Checkout-Tags "$Vulkan_Vendor_Dir/SPIRV-Headers" "sdk-1.3.231.1"

    [string] $build_dir = "$Vulkan_Vendor_Dir/SPIRV-Headers/build"

    Make-Dir $build_dir

    cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="$build_dir/install" -S"$Vulkan_Vendor_Dir/SPIRV-Headers" -B"$build_dir"

    cmake --build "$build_dir" --target install --config Release --parallel $env:NUMBER_OF_PROCESSORS
}

function Setup-Spirv-Tools {
    Update-Submodule vulkan/SPIRV-Tools

    Checkout-Tags "$Vulkan_Vendor_Dir/SPIRV-Tools" "$Spirv_Version"

    [string] $build_dir = "$Vulkan_Vendor_Dir/SPIRV-Tools/build"

    Make-Dir $build_dir

    python3 "$Vulkan_Vendor_Dir\SPIRV-Tools\utils\git-sync-deps"
    cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="$build_dir\install" -S"$Vulkan_Vendor_Dir\SPIRV-Tools" -B"$build_dir"
    cmake --build $build_dir --parallel $env:NUMBER_OF_PROCESSORS --target install --config Release
}

function Setup-Validation-Layers {
    Update-Submodule vulkan/Vulkan-ValidationLayers

    Checkout-Tags "$Vulkan_Vendor_Dir/Vulkan-ValidationLayers" "$Vulkan_Version"

    [string] $build_dir = "$Vulkan_Vendor_Dir/Vulkan-ValidationLayers/build"

    Make-Dir $build_dir

    cmake `
        -DVULKAN_HEADERS_INSTALL_DIR="$Vulkan_Vendor_Dir/Vulkan-Headers/build/install"              `
        -DVULKAN_LOADER_INSTALL_DIR="$Vulkan_Vendor_Dir/Vulkan-Loader/build"                        `
        -DGLSLANG_INSTALL_DIR="$Vulkan_Vendor_Dir/glslang/build/install"                            `
        -DSPIRV_HEADERS_INSTALL_DIR="$Vulkan_Vendor_Dir/SPIRV-Headers/build/install"                `
        -DSPIRV_TOOLS_INSTALL_DIR="$Vulkan_Vendor_Dir/SPIRV-Tools/build/install"                    `
        -DROBIN_HOOD_HASHING_INSTALL_DIR="$Vulkan_Vendor_Dir/robin-hood-hashing/build/install"      `
        -DCMAKE_BUILD_TYPE=Release                                                                  `
        -DCMAKE_INSTALL_PREFIX="$build_dir"                                                         `
        -DBUILD_TESTS=OFF                                                                           `
        -S"$Vulkan_Vendor_Dir/Vulkan-ValidationLayers"                                              `
        -B"$build_dir"

    cmake --build "$build_dir" --config Release --target install -j"$env:NUMBER_OF_PROCESSORS"

    Make-Dir "$Vulkan_Lib_Dir/explicit_layer.d"

    Copy-Item -Path "$build_dir/layers/Release/*" -Destination "$Vulkan_Lib_Dir/explicit_layer.d" -Include "*.json" -Recurse
    Copy-Item -Path "$build_dir/layers/Release/*" -Destination "$Vulkan_Lib_Dir/explicit_layer.d" -Include "*.dll" -Recurse
}

Write-Output "Setting up Dependencies"

if (Test-Path "$Vulkan_Lib_Dir") { Remove-Item -Path "$Vulkan_Lib_Dir" -Recurse -Force }
if (Test-Path "$Vulkan_Vendor_Dir/vulkan/include") { Remove-Item -Path "$Vulkan_Vendor_Dir/vulkan/include" -Recurse -Force }
if (Test-Path "$Vendor_Dir/include") { Remove-Item -Path "$Vendor_Dir/include" -Recurse -Force }

Write-Output "Setting up Raylib"
Write-Output "Cloning Raylib"
Update-Submodule -Submodule raylib-cpp

Write-Output "Setting up utest"
Write-Output "Cloning utest"
Update-Submodule -Submodule utest.h

Write-Output "Setting up zLib"
Setup-Zlib

Write-Output "Setting up libpng"
Setup-LibPng

Write-Output "Setting up freetype"
Setup-FreeType

Write-Output "Setting up glfw"
Setup-Glfw

Write-Output "Setting up glslang"
Setup-Glslang

Write-Output "Setting up volk"
Setup-Volk

Write-Output "Setting up Vulkan-Headers"
Setup-Vulkan-Headers

Write-Output "Setting up Vulkan-Loader"
Setup-Vulkan_Loader

if ($Include_Validation_Layers)
{
    Write-Output "Setting up Robin Hood Hashing"
    Setup-Robin-Hood-Hashing

    Write-Output "Setting up Spirv-Headers"
    Setup-Spirv-Headers

    Write-Output "Setting up Spirv-Tools"
    Setup-Spirv-Tools

    Write-Output "Setting up Vulkan Validation Layers"
    Setup-Validation-Layers

    if (!(Test-Path "$Root_Dir/.env")) { New-Item -Path "$Root_Dir/.env" -ItemType file }
    Set-Content -Path "$Root_Dir/.env" -Value "VK_LAYER_PATH=$Bin_Dir\examples\render\build\lib\explicit_layer.d"
}

Write-Output "Configuring environment file"