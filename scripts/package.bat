@ECHO OFF

set argC=0
for %%x in (%*) do Set /A argC+=1

if %argC% LSS 3  CALL :WriteError && EXIT /B %ERRORLEVEL%

set PKG_NAME=%~1
set OUTPUT_DIR=%~2
set OUTPUT_DIR=%OUTPUT_DIR:/=\%
set BIN_DIR=%~3
set BIN_DIR=%BIN_DIR:/=\%
set PKG_FILE="%OUTPUT_DIR%\%PKG_NAME%"

IF exist %OUTPUT_DIR% rmdir /Q /S "%OUTPUT_DIR%"

mkdir "%PKG_FILE%"

robocopy "%BIN_DIR%\lib" "%PKG_FILE%" * /E
 
echo "%*"| find "--include-validation-layers" >nul

IF "%ERRORLEVEL%" EQU "1" CALL :DeleteDebugSymbols

robocopy "%BIN_DIR%\shaders" "%PKG_FILE%\shaders" * /E
robocopy "%BIN_DIR%\assets" "%PKG_FILE%\assets" * /E
robocopy "%BIN_DIR%" "%PKG_FILE%" "app.exe"
rename %PKG_FILE%\app.exe %PKG_NAME%.exe

EXIT /B %ERRORLEVEL%

:DeleteDebugSymbols
    del %PKG_FILE%\VkLayer_khronos_validation.dll
    rmdir /S /Q %PKG_FILE%\explicit_layer.d
EXIT /B 0

:WriteError
    echo "Requires three arguments (e.g. package.bat <package_name> <output_dir> <bin_dir> [--include-validation-layers])"
EXIT /B 0

