@ECHO OFF

setlocal enableextensions

set ROOT_DIR=%cd%
set OS=Windows
set GENERATOR=MinGW Makefiles

set SUBMODULE_DIR=%ROOT_DIR%\vendor
set SUBMODULE_INCLUDE_DIR=%SUBMODULE_DIR%\include
set SUBMODULE_LIB_DIR=%ROOT_DIR%\lib\%OS%

set BUILD_TYPE=%~1

set LOADER_INSTALL_DIR=%SUBMODULE_DIR%\Vulkan-Loader\build\loader\Release

echo "Installing vulkan dependencies..."

CALL :SetupVolk
CALL :SetupGlfw

if NOT DEFINED VULKAN_SDK CALL :SetupHeadersAndLoader else echo "Vulkan_SDK detected. Using SDK resources for build"

EXIT /B %ERRORLEVEL%

:UpdateSubmodule
    git submodule update --init %SUBMODULE_DIR%/%~1
EXIT /B 0

:SetupVolk 
    echo "Cloning volk..."

    CALL :UpdateSubmodule volk

    mkdir %SUBMODULE_DIR%\include\volk

    robocopy %SUBMODULE_DIR%\volk %SUBMODULE_INCLUDE_DIR%/volk volk.h
    robocopy %SUBMODULE_DIR%\volk %SUBMODULE_INCLUDE_DIR%/volk volk.c
EXIT /B 0

:SetupGlfw
    echo "Cloning glfw..."

    CALL :UpdateSubmodule glfw

    cmake -G "%GENERATOR%" -B%SUBMODULE_DIR%\glfw -S%SUBMODULE_DIR%\glfw 
    mingw32-make -C %SUBMODULE_DIR%\glfw  -j%NUMBER_OF_PROCESSORS%

    mkdir %SUBMODULE_LIB_DIR%

    robocopy %SUBMODULE_DIR%\glfw\src %SUBMODULE_LIB_DIR% libglfw3.a
EXIT /B 0

:SetupVulkanHeaders
    echo "Cloning Vulkan Headers..."

    CALL :UpdateSubmodule Vulkan-Headers

    git -C %SUBMODULE_DIR%\Vulkan-Headers fetch --all --tags && git -C %SUBMODULE_DIR%\Vulkan-Headers checkout tags/v1.3.211
    
    mkdir %SUBMODULE_DIR%\Vulkan-Headers\build

    cmake -DCMAKE_INSTALL_PREFIX=%SUBMODULE_DIR%\Vulkan-Headers\build\install -G "%GENERATOR%" -B%SUBMODULE_DIR%\Vulkan-Headers\build -S%SUBMODULE_DIR%\Vulkan-Headers
    cmake --build %SUBMODULE_DIR%\Vulkan-Headers\build --target install

    mkdir %SUBMODULE_INCLUDE_DIR%\vulkan

    robocopy %SUBMODULE_DIR%\Vulkan-Headers\include\vulkan %SUBMODULE_INCLUDE_DIR%\vulkan **.h
EXIT /B 0

:SetupVulkanLoader 
    echo "Cloning Vulkan Loader..."

    CALL :UpdateSubmodule Vulkan-Loader
    
    mkdir %SUBMODULE_DIR%\Vulkan-Loader\build
    
    cmake -DCMAKE_INSTALL_PREFIX=%SUBMODULE_DIR%\Vulkan-Loader\build -DVULKAN_HEADERS_INSTALL_DIR=%SUBMODULE_DIR%\Vulkan-Headers\build\install -S%SUBMODULE_DIR%\Vulkan-Loader -B%SUBMODULE_DIR%\Vulkan-Loader\build
    cmake --build %SUBMODULE_DIR%\Vulkan-Loader\build --config Release

    mkdir %SUBMODULE_LIB_DIR%

    robocopy %LOADER_INSTALL_DIR% %SUBMODULE_LIB_DIR% **.dll
EXIT /B 0

:SetupHeadersAndLoader
    echo "VULKAN_SDK not detected. Proceeding to pull Vulkan Headers and Loader..."
    CALL :SetupVulkanHeaders 
    CALL :SetupVulkanLoader
    echo # Environment variables for Vulkan.> .env
    echo DYLD_LIBRARY_PATH=%SUBMODULE_LIB_DIR%>> .env
    echo VULKAN_INCLUDE_DIR=%SUBMODULE_INCLUDE_DIR%/vulkan>> .env

    if "%BUILD_TYPE%" == "DEBUG" CALL :SetupVulkanValidationLayers
EXIT /B 0

:SetupGlslang
    echo "Cloning glslang..."

    CALL :UpdateSubmodule glslang

    mkdir %SUBMODULE_DIR%\glslang\build

    cmake -G "%GENERATOR%" -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=%SUBMODULE_DIR%\glslang\build\install -S%SUBMODULE_DIR%\glslang -B%SUBMODULE_DIR%\glslang\build
    cmake --build %SUBMODULE_DIR%\glslang\build --target install -- -j%NUMBER_OF_PROCESSORS%

    mkdir %SUBMODULE_LIB_DIR%
    
    robocopy %SUBMODULE_DIR%\glslang\build\glslang %SUBMODULE_LIB_DIR% libglslang.a
EXIT /B 0

:SetupSpirvHeaders
    echo "Cloning SPIRV-Headers..."

    CALL :UpdateSubmodule debug\SPIRV-Headers

    mkdir %SUBMODULE_DIR%\debug\SPIRV-Headers\build

    cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=%SUBMODULE_DIR%\debug\SPIRV-Headers\build\install -S%SUBMODULE_DIR%\debug\SPIRV-Headers -B%SUBMODULE_DIR%\debug\SPIRV-Headers\build
    cmake --build %SUBMODULE_DIR%\debug\SPIRV-Headers\build --target install --config Release
EXIT /B 0

:SetupSpirvTools
    echo "Cloning SPIRV-Tools..."

    CALL :UpdateSubmodule debug\SPIRV-Tools

    mkdir %SUBMODULE_DIR%\debug\SPIRV-Tools\build

    python3 %SUBMODULE_DIR%\debug\SPIRV-Tools\utils\git-sync-deps
    cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=%SUBMODULE_DIR%\debug\SPIRV-Tools\build\install -S%SUBMODULE_DIR%\debug\SPIRV-Tools -B%SUBMODULE_DIR%\debug\SPIRV-Tools\build
    cmake --build %SUBMODULE_DIR%\debug\SPIRV-Tools\build --target install --config Release
EXIT /B 0

:SetupRobinHoodHashing 
    echo "Cloning robin-hood-hashing..."

    CALL :UpdateSubmodule debug/robin-hood-hashing

    cmake -G "%GENERATOR%" %SUBMODULE_DIR%\debug\robin-hood-hashing -DCMAKE_INSTALL_PREFIX=%SUBMODULE_DIR%\debug\robin-hood-hashing\build\install -DRH_STANDALONE_PROJECT=OFF -DCMAKE_BUILD_TYPE=Release -S%SUBMODULE_DIR%\debug\robin-hood-hashing -B%SUBMODULE_DIR%\debug\robin-hood-hashing\build
    cmake --build %SUBMODULE_DIR%\debug\robin-hood-hashing\build --target install -- -j%NUMBER_OF_PROCESSORS%
EXIT /B 0

:SetupValidationLayers 
    echo "Cloning Vulkan Validation Layers..."

    CALL :UpdateSubmodule debug\Vulkan-ValidationLayers
    
    git -C %SUBMODULE_DIR%\debug\Vulkan-ValidationLayers fetch --all --tags && git -C %SUBMODULE_DIR%\debug\Vulkan-ValidationLayers checkout tags/v1.3.211

    mkdir %SUBMODULE_DIR%\debug\Vulkan-ValidationLayers\build
    
    cmake -DVULKAN_HEADERS_INSTALL_DIR=%SUBMODULE_DIR%\Vulkan-Headers\build\install ^
        -DVULKAN_LOADER_INSTALL_DIR=%SUBMODULE_DIR%\Vulkan-Loader\build ^
        -DGLSLANG_INSTALL_DIR=%SUBMODULE_DIR%\glslang\build\install ^
        -DSPIRV_HEADERS_INSTALL_DIR=%SUBMODULE_DIR%\debug\SPIRV-Headers\build\install ^
        -DSPIRV_TOOLS_INSTALL_DIR=%SUBMODULE_DIR%\debug\SPIRV-Tools\build\install ^
        -DROBIN_HOOD_HASHING_INSTALL_DIR=%SUBMODULE_DIR%\debug\robin-hood-hashing\build\install ^
        -DCMAKE_BUILD_TYPE=Release ^
        -DCMAKE_INSTALL_PREFIX=. ^
        -DBUILD_TESTS=OFF ^
        -S%SUBMODULE_DIR%\debug\Vulkan-ValidationLayers ^
        -B%SUBMODULE_DIR%\debug\Vulkan-ValidationLayers\build

    cmake --build %SUBMODULE_DIR%\debug\Vulkan-ValidationLayers\build --config Release --target install

    mkdir %SUBMODULE_LIB_DIR%\explicit_layer.d

    robocopy %SUBMODULE_DIR%\debug\Vulkan-ValidationLayers\build\layers\Release %SUBMODULE_LIB_DIR%\explicit_layer.d **.json
    robocopy %SUBMODULE_DIR%\debug\Vulkan-ValidationLayers\build\layers\Release %SUBMODULE_LIB_DIR%\explicit_layer.d **.dll
    robocopy %SUBMODULE_LIB_DIR%\explicit_layer.d %SUBMODULE_LIB_DIR% **.dll
EXIT /B 0

:SetupVulkanValidationLayers 
    echo "Building Vulkan Validation Layers and dependencies..."
    CALL :SetupGlslang
    CALL :SetupRobinHoodHashing
    CALL :SetupSpirvHeaders
    CALL :SetupSpirvTools
    CALL :SetupValidationLayers
    echo VK_LAYER_PATH=%SUBMODULE_LIB_DIR%\explicit_layer.d>> .env
EXIT /B 0

endlocal 
