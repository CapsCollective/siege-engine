@ECHO OFF

:: ===================================================================
:: Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
::
:: This code is released under an unmodified zlib license.
:: For conditions of distribution and use, please see:
::     https://opensource.org/licenses/Zlib
:: ===================================================================

setlocal enableextensions

set ROOT_DIR=%cd%

set VULKAN_VERSION=v1.3.211
set SPIRV_VERSION=v2022.4
set GENERATOR=MinGW Makefiles
set VENDOR_DIR=%ROOT_DIR%\vendor

set BIN_DIR=%ROOT_DIR%\bin
set BUILD_DIR=examples\render\build

set VULKAN_VENDOR_DIR=%VENDOR_DIR%\vulkan
set VULKAN_LIB_DIR=%VULKAN_VENDOR_DIR%\lib
set VULKAN_INCLUDE_DIR=%VULKAN_VENDOR_DIR%\include\vulkan
set VOLK_INCLUDE_DIR=%VULKAN_VENDOR_DIR%\include\volk

echo "Setting up dependencies..."

echo "Setting up raylib..."
echo "Cloning raylib..."
git submodule update --init --recursive %VENDOR_DIR%/raylib-cpp

echo "Setting up utest..."
echo "Cloning utest..."
git submodule update --init --recursive %VENDOR_DIR%/utest.h

echo "Setting up GLFW..."
CALL :SetupGlfw

echo "Setting up volk..."
CALL :SetupVolk

echo "Setting up GLSLang..."
CALL :SetupGlslang
CALL :SetupGlm

echo "Setting up Vulkan Headers"
CALL :SetupVulkanHeaders

echo "Setting up Vulkan Loader"
CALL :SetupVulkanLoader

echo "Setting up ZLib"
CALL :SetupZlib

echo "Setting up LibPng"
CALL :SetupLibPng

echo "Setting up freeType"
CALL :SetupFreeType

echo "%*"| find "--include-validation-layers" >nul

IF "%ERRORLEVEL%" EQU "0" CALL :SetupVulkanValidationLayers

echo "Configuring environment file..."

echo VK_LAYER_PATH=%BIN_DIR%\examples\render\build\lib\explicit_layer.d> .env

echo "Setup Complete"

EXIT /B %ERRORLEVEL%

:UpdateSubmodule
    git submodule update --init %VENDOR_DIR%/%~1
EXIT /B 0

:CheckoutTags
    git -C %~1 fetch --all --tags && git -C %~1 checkout tags/%~2
EXIT /B 0

:SetupGlfw
    echo "Cloning glfw..."

    CALL :UpdateSubmodule glfw

    cmake -G "%GENERATOR%" -B%VENDOR_DIR%\glfw -S%VENDOR_DIR%\glfw 
    mingw32-make -C %VENDOR_DIR%\glfw  -j%NUMBER_OF_PROCESSORS%
EXIT /B 0

:SetupVolk
    echo "Cloning volk..."

    CALL :UpdateSubmodule vulkan\volk

    mkdir %VOLK_INCLUDE_DIR%

    robocopy %VULKAN_VENDOR_DIR%\volk %VOLK_INCLUDE_DIR% volk.h
    robocopy %VULKAN_VENDOR_DIR%\volk %VOLK_INCLUDE_DIR% volk.c
EXIT /B 0

:SetupGlslang
    echo "Cloning glslang..."

    CALL :UpdateSubmodule glslang

    mkdir %VENDOR_DIR%\glslang\build

    cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=%VENDOR_DIR%\glslang\build\install -S%VENDOR_DIR%\glslang -B%VENDOR_DIR%\glslang\build
    cmake --build %VENDOR_DIR%\glslang\build --parallel %NUMBER_OF_PROCESSORS% --target install
EXIT /B 0

:SetupGlm
    echo "Cloning glm..."

    CALL :UpdateSubmodule glm
EXIT /B 0

:SetupVulkanHeaders
    echo "Cloning Vulkan Headers..."

    CALL :UpdateSubmodule vulkan\Vulkan-Headers

    CALL :CheckoutTags %VULKAN_VENDOR_DIR%\Vulkan-Headers %VULKAN_VERSION%

    git -C %VULKAN_VENDOR_DIR%\Vulkan-Headers fetch --all --tags && git -C %VULKAN_VENDOR_DIR%\Vulkan-Headers checkout tags/v1.3.211

    set BUILD_DIR=%VULKAN_VENDOR_DIR%\Vulkan-Headers\build

    mkdir %BUILD_DIR%

    cmake -DCMAKE_INSTALL_PREFIX=%BUILD_DIR%\install -B%BUILD_DIR% -S%VULKAN_VENDOR_DIR%\Vulkan-Headers
    cmake --build %BUILD_DIR% --parallel %NUMBER_OF_PROCESSORS% --target install

    mkdir %VULKAN_INCLUDE_DIR%

    robocopy %VULKAN_VENDOR_DIR%\Vulkan-Headers\include\vulkan %VULKAN_INCLUDE_DIR% **.h
EXIT /B 0

:SetupFreeType
    echo "Cloning freeType..."

    CALL :UpdateSubmodule freetype

    set BUILD_DIR=%VENDOR_DIR%\freetype\build
    set ZLIB_BUILD_DIR=%VENDOR_DIR%\zlib\build
    set LIBPNG_BUILD_DIR=%VENDOR_DIR%\libpng

    cmake -G "%GENERATOR%" -DFT_DISABLE_BROTLI=TRUE -DFT_DISABLE_BZIP2=TRUE -DFT_DISABLE_HARFBUZZ=TRUE -DCMAKE_INSTALL_PREFIX=%BUILD_DIR% -DZLIB_LIBRARY=%ZLIB_BUILD_DIR%\lib\libz.a -DZLIB_INCLUDE_DIR=%ZLIB_BUILD_DIR%\include -DPNG_LIBRARY=%LIBPNG_BUILD_DIR%\libpng.a -DPNG_PNG_INCLUDE_DIR=%LIBPNG_BUILD_DIR% -B%BUILD_DIR% -S%VENDOR_DIR%\freetype -DCMAKE_SH="CMAKE_SH-NOTFOUND"
    cmake --build %BUILD_DIR% --config Release --target install

    mkdir %VENDOR_DIR%\include\freetype

    XCopy %BUILD_DIR%\include\freetype2 %VENDOR_DIR%\include\freetype /s /i /y
EXIT /B 0

:SetupZlib
    echo "Cloning zlib..."

    CALL :UpdateSubmodule zlib

    set BUILD_DIR=%VENDOR_DIR%\zlib\build

    mkdir %BUILD_DIR%

    make -C %VENDOR_DIR%\zlib -f %VENDOR_DIR%\zlib\win32\Makefile.gcc install INCLUDE_PATH=%BUILD_DIR%\include LIBRARY_PATH=%BUILD_DIR%\lib BINARY_PATH=%BUILD_DIR%\bin
EXIT /B 0

:SetupLibPng
    echo "Cloning libpng..."

    CALL :UpdateSubmodule libpng

    set BUILD_DIR=%VENDOR_DIR%\libpng\build

    mkdir %BUILD_DIR%

    make -C %VENDOR_DIR%\libpng -f %VENDOR_DIR%\libpng\scripts\makefile.gcc ZLIBINC=%VENDOR_DIR%\zlib\build\include ZLIBLIB=%VENDOR_DIR%\zlib\build\lib
EXIT /B 0

:SetupVulkanLoader
    echo "Cloning Vulkan Loader..."

    CALL :UpdateSubmodule vulkan\Vulkan-Loader

    CALL :CheckoutTags %VULKAN_VENDOR_DIR%\Vulkan-Loader %VULKAN_VERSION%

    set BUILD_DIR=%VULKAN_VENDOR_DIR%\Vulkan-Loader\build

    mkdir %BUILD_DIR%

    cmake -DCMAKE_INSTALL_PREFIX=%BUILD_DIR% -DVULKAN_HEADERS_INSTALL_DIR=%VULKAN_VENDOR_DIR%\Vulkan-Headers\build\install -S%VULKAN_VENDOR_DIR%\Vulkan-Loader -B%BUILD_DIR%
    cmake --build %BUILD_DIR% --parallel %NUMBER_OF_PROCESSORS% --config Release

    mkdir %VULKAN_LIB_DIR%

    robocopy %BUILD_DIR%\loader\Release %VULKAN_LIB_DIR% **.dll
EXIT /B 0

:SetupSpirvHeaders
    echo "Cloning SPIRV-Headers..."

    CALL :UpdateSubmodule vulkan\SPIRV-Headers

    CALL :CheckoutTags %VULKAN_VENDOR_DIR%\SPIRV-Headers sdk-1.3.231.1

    set BUILD_DIR=%VULKAN_VENDOR_DIR%\SPIRV-Headers\build

    mkdir %BUILD_DIR%

    cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=%BUILD_DIR%\install -S%VULKAN_VENDOR_DIR%\SPIRV-Headers -B%BUILD_DIR%
    cmake --build %BUILD_DIR% --parallel %NUMBER_OF_PROCESSORS% --target install --config Release
EXIT /B 0

:SetupSpirvTools
    echo "Cloning SPIRV-Tools..."

    CALL :UpdateSubmodule vulkan\SPIRV-Tools

    CALL :CheckoutTags %VULKAN_VENDOR_DIR%\SPIRV-Tools %SPIRV_VERSION%

    set BUILD_DIR=%VULKAN_VENDOR_DIR%\SPIRV-Tools\build

    mkdir %BUILD_DIR%

    python3 %VULKAN_VENDOR_DIR%\SPIRV-Tools\utils\git-sync-deps
    cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=%BUILD_DIR%\install -DCMAKE_BUILD_PARALLEL_LEVEL=%NUMBER_OF_PROCESSORS% -S%VULKAN_VENDOR_DIR%\SPIRV-Tools -B%BUILD_DIR%
    cmake --build %BUILD_DIR% --parallel %NUMBER_OF_PROCESSORS% --target install --config Release
EXIT /B 0

:SetupRobinHoodHashing
    echo "Cloning robin-hood-hashing..."

    CALL :UpdateSubmodule vulkan/robin-hood-hashing

    set BUILD_DIR=%VULKAN_VENDOR_DIR%\robin-hood-hashing\build

    cmake %VULKAN_VENDOR_DIR%\robin-hood-hashing -DCMAKE_INSTALL_PREFIX=%BUILD_DIR%\install -DRH_STANDALONE_PROJECT=OFF -DCMAKE_BUILD_TYPE=Release -S%VULKAN_VENDOR_DIR%\robin-hood-hashing -B%BUILD_DIR%
    cmake --build %BUILD_DIR% --parallel %NUMBER_OF_PROCESSORS% --target install
EXIT /B 0

:SetupValidationLayers
    echo "Cloning Vulkan Validation Layers..."

    CALL :UpdateSubmodule vulkan\Vulkan-ValidationLayers

    CALL :CheckoutTags %VULKAN_VENDOR_DIR%\Vulkan-ValidationLayers %VULKAN_VERSION%

    set BUILD_DIR=%VULKAN_VENDOR_DIR%\Vulkan-ValidationLayers\build

    cmake -DVULKAN_HEADERS_INSTALL_DIR=%VULKAN_VENDOR_DIR%\Vulkan-Headers\build\install ^
        -DVULKAN_LOADER_INSTALL_DIR=%VULKAN_VENDOR_DIR%\Vulkan-Loader\build ^
        -DGLSLANG_INSTALL_DIR=%VENDOR_DIR%\glslang\build\install ^
        -DSPIRV_HEADERS_INSTALL_DIR=%VULKAN_VENDOR_DIR%\SPIRV-Headers\build\install ^
        -DSPIRV_TOOLS_INSTALL_DIR=%VULKAN_VENDOR_DIR%\SPIRV-Tools\build\install ^
        -DROBIN_HOOD_HASHING_INSTALL_DIR=%VULKAN_VENDOR_DIR%\robin-hood-hashing\build\install ^
        -DCMAKE_BUILD_TYPE=Release ^
        -DCMAKE_INSTALL_PREFIX=%BUILD_DIR% ^
        -DBUILD_TESTS=OFF ^
        -S%VULKAN_VENDOR_DIR%\Vulkan-ValidationLayers ^
        -B%BUILD_DIR%

    cmake --build %BUILD_DIR% --parallel %NUMBER_OF_PROCESSORS% --config Release --target install

    mkdir %VULKAN_LIB_DIR%\explicit_layer.d

    robocopy %BUILD_DIR%\layers\Release %VULKAN_LIB_DIR%\explicit_layer.d **.json
    robocopy %BUILD_DIR%\layers\Release %VULKAN_LIB_DIR%\explicit_layer.d **.dll
EXIT /B 0

:SetupVulkanValidationLayers
    echo "Building Vulkan Validation Layers and dependencies..."

    CALL :SetupRobinHoodHashing
    CALL :SetupSpirvHeaders
    CALL :SetupSpirvTools
    CALL :SetupValidationLayers
EXIT /B 0

endlocal 
