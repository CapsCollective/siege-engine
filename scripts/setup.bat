@ECHO OFF

setlocal enableextensions

set ROOT_DIR=%cd%
set GENERATOR=MinGW Makefiles
set VENDOR_DIR=%ROOT_DIR%\vendor
set BIN_DIR=%ROOT_DIR%\bin

# Vulkan dependency variables
set VULKAN_VENDOR_DIR=%VENDOR_DIR%\vulkan
set VULKAN_LIB_DIR=%VULKAN_VENDOR_DIR%\lib
set VULKAN_INCLUDE_DIR=%VULKAN_VENDOR_DIR%\include\vulkan
set VOLK_INCLUDE_DIR=%VULKAN_VENDOR_DIR%\include\volk

set DEBUG=%~1

echo "Setting up dependencies..."

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

if "%DEBUG%" EQU "DEBUG" CALL :SetupVulkanValidationLayers

EXIT /B %ERRORLEVEL%

:UpdateSubmodule
    git submodule update --init %VENDOR_DIR%/%~1
EXIT /B 0

:CheckoutTags
    git -C %~1 fetch --all --tags && git -C %~1 checkout tags/v%~2
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

    cmake -G "%GENERATOR%" -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=%VENDOR_DIR%\glslang\build\install -S%VENDOR_DIR%\glslang -B%VENDOR_DIR%\glslang\build
    cmake --build %VENDOR_DIR%\glslang\build --target install -- -j%NUMBER_OF_PROCESSORS%
EXIT /B 0

:SetupGlm
    echo "Cloning glm..."

    CALL :UpdateSubmodule glm
EXIT /B 0

:SetupVulkanHeaders
    echo "Cloning Vulkan Headers..."

    CALL :UpdateSubmodule vulkan\Vulkan-Headers

    CALL :CheckoutTags %VULKAN_VENDOR_DIR%\Vulkan-Headers 1.3.211

    git -C %VULKAN_VENDOR_DIR%\Vulkan-Headers fetch --all --tags && git -C %VULKAN_VENDOR_DIR%\Vulkan-Headers checkout tags/v1.3.211

    set BUILD_DIR=%VULKAN_VENDOR_DIR%\Vulkan-Headers\build

    mkdir %BUILD_DIR%

    cmake -DCMAKE_INSTALL_PREFIX=%BUILD_DIR%\install -G "%GENERATOR%" -B%BUILD_DIR% -S%VULKAN_VENDOR_DIR%\Vulkan-Headers
    cmake --build %BUILD_DIR% --target install

    mkdir %VULKAN_INCLUDE_DIR%

    robocopy %VULKAN_VENDOR_DIR%\Vulkan-Headers\include\vulkan %VULKAN_INCLUDE_DIR% **.h
EXIT /B 0

:SetupVulkanLoader
    echo "Cloning Vulkan Loader..."

    CALL :UpdateSubmodule vulkan\Vulkan-Loader

    CALL :CheckoutTags %VULKAN_VENDOR_DIR%\Vulkan-Loader 1.3.211

    set BUILD_DIR=%VULKAN_VENDOR_DIR%\Vulkan-Loader\build

    mkdir %BUILD_DIR%

    cmake -DCMAKE_INSTALL_PREFIX=%BUILD_DIR% -DVULKAN_HEADERS_INSTALL_DIR=%VULKAN_VENDOR_DIR%\Vulkan-Headers\build\install -S%VULKAN_VENDOR_DIR%\Vulkan-Loader -B%BUILD_DIR%
    cmake --build %BUILD_DIR% --config Release

    mkdir %VULKAN_LIB_DIR%

    robocopy %BUILD_DIR%\loader\Release %VULKAN_LIB_DIR% **.dll
EXIT /B 0

:SetupSpirvHeaders
    echo "Cloning SPIRV-Headers..."

    CALL :UpdateSubmodule vulkan\SPIRV-Headers

    set BUILD_DIR=%VULKAN_VENDOR_DIR%\SPIRV-Headers\build

    mkdir %BUILD_DIR%

    cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=%BUILD_DIR%\install -S%VULKAN_VENDOR_DIR%\SPIRV-Headers -B%BUILD_DIR%
    cmake --build %BUILD_DIR% --target install --config Release
EXIT /B 0

:SetupSpirvTools
    echo "Cloning SPIRV-Tools..."

    CALL :UpdateSubmodule vulkan\SPIRV-Tools

    set BUILD_DIR=%VULKAN_VENDOR_DIR%\SPIRV-Tools\build

    mkdir %BUILD_DIR%

    python3 %VULKAN_VENDOR_DIR%\SPIRV-Tools\utils\git-sync-deps
    cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=%BUILD_DIR%\install -S%VULKAN_VENDOR_DIR%\SPIRV-Tools -B%BUILD_DIR%
    cmake --build %BUILD_DIR% --target install --config Release
EXIT /B 0

:SetupRobinHoodHashing
    echo "Cloning robin-hood-hashing..."

    CALL :UpdateSubmodule vulkan/robin-hood-hashing

    set BUILD_DIR=%VULKAN_VENDOR_DIR%\robin-hood-hashing\build

    cmake -G "%GENERATOR%" %VULKAN_VENDOR_DIR%\robin-hood-hashing -DCMAKE_INSTALL_PREFIX=%BUILD_DIR%\install -DRH_STANDALONE_PROJECT=OFF -DCMAKE_BUILD_TYPE=Release -S%VULKAN_VENDOR_DIR%\robin-hood-hashing -B%BUILD_DIR%
    cmake --build %BUILD_DIR% --target install -- -j%NUMBER_OF_PROCESSORS%
EXIT /B 0

:SetupValidationLayers
    echo "Cloning Vulkan Validation Layers..."

    CALL :UpdateSubmodule vulkan\Vulkan-ValidationLayers

    CALL :CheckoutTags %VULKAN_VENDOR_DIR%\Vulkan-ValidationLayers 1.3.211

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

    cmake --build %BUILD_DIR% --config Release --target install

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

    echo VK_LAYER_PATH=%BIN_DIR%\lib\explicit_layer.d> .env
EXIT /B 0

endlocal 
