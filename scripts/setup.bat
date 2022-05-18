@ECHO OFF

setlocal enableextensions

set ROOT_DIR=%cd%
set GENERATOR=MinGW Makefiles

set VENDOR_DIR=%ROOT_DIR%\vendor

set BUILD_TYPE=%~1

echo "Setting up dependencies..."

CALL :SetupGlfw
CALL :SetupGlslang

echo VK_LAYER_PATH=%VENDOR_DIR%\vulkan\lib\windows\explicit_layer.d>> .env

EXIT /B %ERRORLEVEL%

:UpdateSubmodule
    git submodule update --init %VENDOR_DIR%/%~1
EXIT /B 0

:SetupGlfw
    echo "Cloning glfw..."

    CALL :UpdateSubmodule glfw

    cmake -G "%GENERATOR%" -B%VENDOR_DIR%\glfw -S%VENDOR_DIR%\glfw 
    mingw32-make -C %VENDOR_DIR%\glfw  -j%NUMBER_OF_PROCESSORS%
EXIT /B 0

:SetupGlslang
    echo "Cloning glslang..."

    CALL :UpdateSubmodule glslang

    mkdir %VENDOR_DIR%\glslang\build

    cmake -G "%GENERATOR%" -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=%VENDOR_DIR%\glslang\build\install -S%VENDOR_DIR%\glslang -B%VENDOR_DIR%\glslang\build
    cmake --build %VENDOR_DIR%\glslang\build --target install -- -j%NUMBER_OF_PROCESSORS%
EXIT /B 0

endlocal 
