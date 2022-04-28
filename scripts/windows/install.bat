@ECHO OFF

setlocal enableextensions

set ROOT_DIR=%cd%
set OS=Windows
set GENERATOR=MinGW Makefiles

set SUBMODULE_DIR=%ROOT_DIR%\vendor
set SUBMODULE_INCLUDE_DIR=%SUBMODULE_DIR%\include
set SUBMODULE_LIB_DIR=%ROOT_DIR%\lib\%OS%

set LOADER_INSTALL_DIR=%SUBMODULE_DIR%\Vulkan-Loader\build\loader\Release

echo "Installing vulkan dependencies..."

CALL :SetupVolk
CALL :SetupGlfw

if NOT DEFINED VULKAN_SDK CALL :SetupHeadersAndLoader if %1 == DEBUG CALL :SetupVulkanValidationLayers

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

    cd %SUBMODULE_DIR%\glfw && cmake -G "%GENERATOR%" . && mingw32-make -j%NUMBER_OF_PROCESSORS%

    mkdir %SUBMODULE_LIB_DIR%

    robocopy %SUBMODULE_DIR%\glfw\src %SUBMODULE_LIB_DIR% libglfw3.a

    cd %ROOT_DIR%
EXIT /B 0

:SetupVulkanHeaders
    echo "Cloning Vulkan Headers..."

    CALL :UpdateSubmodule Vulkan-Headers

    cd %SUBMODULE_DIR%\Vulkan-Headers && git fetch --all --tags && git checkout tags/v1.3.211
    
    mkdir %SUBMODULE_DIR%\Vulkan-Headers\build

    cd %SUBMODULE_DIR%\Vulkan-Headers\build && cmake -DCMAKE_INSTALL_PREFIX=install -G "%GENERATOR%" ..
    cd %SUBMODULE_DIR%\Vulkan-Headers\build && cmake --build . --target install

    mkdir %SUBMODULE_INCLUDE_DIR%\vulkan

    robocopy %SUBMODULE_DIR%\Vulkan-Headers\include\vulkan %SUBMODULE_INCLUDE_DIR%\vulkan **.h

    cd %ROOT_DIR%
EXIT /B 0

:SetupVulkanLoader 
    echo "Cloning Vulkan Loader..."

    CALL :UpdateSubmodule Vulkan-Loader
    
    mkdir %SUBMODULE_DIR%\Vulkan-Loader\build
    
    cd %SUBMODULE_DIR%\Vulkan-Loader\build && cmake -DVULKAN_HEADERS_INSTALL_DIR=%SUBMODULE_DIR%\Vulkan-Headers\build\install ..
    cd %SUBMODULE_DIR%\Vulkan-Loader\build && cmake --build . --config Release

    mkdir %SUBMODULE_LIB_DIR%

    robocopy %LOADER_INSTALL_DIR% %SUBMODULE_LIB_DIR% **.dll

    cd %ROOT_DIR%
EXIT /B 0

:SetupHeadersAndLoader
    echo "VULKAN_SDK not detected. Proceeding to pull Vulkan Headers and Loader..."
    CALL :SetupVulkanHeaders 
    CALL :SetupVulkanLoader 
    echo "export DYLD_LIBRARY_PATH=%SUBMODULE_LIB_DIR%" > .env
    echo "export VULKAN_INCLUDE_DIR=%SUBMODULE_INCLUDE_DIR%/vulkan" >> .env
EXIT /B 0

:SetupGlslang
    echo "Cloning glslang..."

    CALL :UpdateSubmodule glslang

    mkdir %SUBMODULE_DIR%\glslang\build

    cd %SUBMODULE_DIR%\glslang\build && cmake -G "%GENERATOR%" -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=%SUBMODULE_DIR%\glslang\build\install ..
    cd %SUBMODULE_DIR%\glslang\build && cmake --build . --target install -- -j%NUMBER_OF_PROCESSORS%

    mkdir %SUBMODULE_LIB_DIR%
    
    robocopy %SUBMODULE_DIR%\glslang\build\glslang %SUBMODULE_LIB_DIR% libglslang.a

    cd %ROOT_DIR%
EXIT /B 0

:SetupSpirvHeaders
    echo "Cloning SPIRV-Headers..."

    CALL :UpdateSubmodule debug\SPIRV-Headers

    mkdir %SUBMODULE_DIR%\debug\SPIRV-Headers\build

    cd %SUBMODULE_DIR%\debug\SPIRV-Headers\build && cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=%SUBMODULE_DIR%\debug\SPIRV-Headers\build\install
    cd %SUBMODULE_DIR%\debug\SPIRV-Headers\build && cmake --build . --target install --config Release

    cd %ROOT_DIR%
EXIT /B 0

:SetupSpirvTools
    echo "Cloning SPIRV-Tools..."

    CALL :UpdateSubmodule debug\SPIRV-Tools

    mkdir %SUBMODULE_DIR%\debug\SPIRV-Tools\build

    cd %SUBMODULE_DIR%\debug\SPIRV-Tools\build && python3 %SUBMODULE_DIR%\debug\SPIRV-Tools\utils\git-sync-deps
    cd %SUBMODULE_DIR%\debug\SPIRV-Tools\build && cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=%SUBMODULE_DIR%\debug\SPIRV-Tools\build\install
    cd %SUBMODULE_DIR%\debug\SPIRV-Tools\build && cmake --build . --target install --config Release

    cd %ROOT_DIR%
EXIT /B 0

:SetupRobinHoodHashing 
    echo "Cloning robin-hood-hashing..."

    CALL :UpdateSubmodule debug/robin-hood-hashing

    cd %SUBMODULE_DIR%\debug\robin-hood-hashing\build && cmake -G "%GENERATOR%" %SUBMODULE_DIR%\debug\robin-hood-hashing -DCMAKE_INSTALL_PREFIX=%SUBMODULE_DIR%\debug\robin-hood-hashing\build\install -DRH_STANDALONE_PROJECT=OFF -DCMAKE_BUILD_TYPE=Release
    cd %SUBMODULE_DIR%\debug\robin-hood-hashing\build && cmake --build %SUBMODULE_DIR%\debug\robin-hood-hashing\build --target install -- -j%NUMBER_OF_PROCESSORS%

    cd %ROOT_DIR%
EXIT /B 0

:SetupValidationLayers 
    echo "Cloning Vulkan Validation Layers..."

    CALL :UpdateSubmodule debug\Vulkan-ValidationLayers
    
    cd %SUBMODULE_DIR%\debug\Vulkan-ValidationLayers && git fetch --all --tags && git checkout tags/v1.3.211

    mkdir %SUBMODULE_DIR%\debug\Vulkan-ValidationLayers\build
    
    cd %SUBMODULE_DIR%\debug\Vulkan-ValidationLayers\build && cmake -DVULKAN_HEADERS_INSTALL_DIR=%SUBMODULE_DIR%\Vulkan-Headers\build\install ^
        -DVULKAN_LOADER_INSTALL_DIR=%SUBMODULE_DIR%\Vulkan-Loader\build ^
        -DGLSLANG_INSTALL_DIR=%SUBMODULE_DIR%\glslang\build\install ^
        -DSPIRV_HEADERS_INSTALL_DIR=%SUBMODULE_DIR%\debug\SPIRV-Headers\build\install ^
        -DSPIRV_TOOLS_INSTALL_DIR=%SUBMODULE_DIR%\debug\SPIRV-Tools\build\install ^
        -DROBIN_HOOD_HASHING_INSTALL_DIR=%SUBMODULE_DIR%\debug\robin-hood-hashing\build\install ^
        -DCMAKE_BUILD_TYPE=Release ^
        -DCMAKE_INSTALL_PREFIX=. ^
        -DBUILD_TESTS=OFF ..

    cd %SUBMODULE_DIR%\debug\Vulkan-ValidationLayers\build && cmake --build . --config Release --target install

    mkdir %SUBMODULE_LIB_DIR%\explicit_layer.d

    robocopy %SUBMODULE_DIR%\debug\Vulkan-ValidationLayers\build\layers\Release %SUBMODULE_LIB_DIR%\explicit_layer.d **.json
    robocopy %SUBMODULE_DIR%\debug\Vulkan-ValidationLayers\build\layers\Release %SUBMODULE_LIB_DIR%\explicit_layer.d **.dll
    robocopy %SUBMODULE_LIB_DIR%\explicit_layer.d %SUBMODULE_LIB_DIR% **.dll

    cd %ROOT_DIR%
EXIT /B 0

:SetupVulkanValidationLayers 
    echo "Building Vulkan Validation Layers and dependencies..."
    CALL :SetupGlslang
    CALL :SetupRobinHoodHashing
    CALL :SetupSpirvHeaders
    CALL :SetupSpirvTools
    CALL :SetupValidationLayers
    echo "export VK_LAYER_PATH=%SUBMODULE_LIB_DIR%\explicit_layer.d" >> .env
EXIT /B 0

endlocal 