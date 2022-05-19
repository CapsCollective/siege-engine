@ECHO OFF

set argC=0
for %%x in (%*) do Set /A argC+=1

if $argC NEQ 4  CALL :WriteError && EXIT /B %ERRORLEVEL%

set PKG_NAME=%~1
set OUTPUT_DIR=%~2
set BIN_DIR=%~3
set DEBUG=%~4
set PKG_FILE="%OUTPUT_DIR%/%PKG_NAME%"

rmdir -r -f %OUTPUT_DIR%
mkdir %PKG_FILE%
robocopy "%BIN_DIR%/lib" "%PKG_FILE%" * /E

if "%DEBUG%" == "0"  CALL :DeleteDebugSymbols

robocopy "%BIN_DIR%/shaders" "%PKG_FILE%/shaders" * /E
robocopy %BIN_DIR% %PKG_FILE% app.exe
rename %PKG_FILE%\app.exe %PKG_NAME%.exe

EXIT /B %ERRORLEVEL%

:DeleteDebugSymbols
    del %PKG_FILE%\VkLayer_khronos_validation.dll
    rmdir /S /Q %PKG_FILE%\explicit_layer.d
EXIT /B 0

:WriteError
    echo "Requires four arguments (e.g. packageapp.bat <package_name> <output_dir> <bin_dir> <debug>)"
EXIT /B 0

