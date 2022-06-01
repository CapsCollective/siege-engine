@ECHO OFF

SETLOCAL ENABLEDELAYEDEXPANSION

set argC=0
for %%x in (%*) do Set /A argC+=1

if %argC% LSS 4  CALL :WriteError && EXIT /B %ERRORLEVEL%

set PKG_NAME=%~1
set EXE_NAME=%~2
set OUTPUT_DIR=%~3
set OUTPUT_DIR=%OUTPUT_DIR:/=\%
set BUILD_DIR=%~4
set BUILD_DIR=%BUILD_DIR:/=\%
set PKG_FILE=%OUTPUT_DIR%\%PKG_NAME%
SET ENV_VARS=""
SET EXCLUDES=""

CALL :loop %*

IF exist %OUTPUT_DIR% rmdir /Q /S %OUTPUT_DIR%

mkdir "%PKG_FILE%"

robocopy "%BUILD_DIR%\lib" "%PKG_FILE%" * /E

mkdir "%PKG_FILE%\assets"
robocopy "%BUILD_DIR%\assets" "%PKG_FILE%\assets" * /E
robocopy "%BUILD_DIR%" "%PKG_FILE%" "%EXE_NAME%.exe"

rem ROBOCOPY RETURNS 1 TO INDICATE ALL FILES WERE RETURNED SUCCESSFULLY.
rem WE THEREFORE NEED TO CONVERT THIS TO ERROR CODE 0.
if %ERRORLEVEL% EQU 1 SET ERRORLEVEL=0

SET VAL=""
IF "%EXCLUDES%" NEQ "" (
    FOR %%x IN (%EXCLUDES%) DO (
        SET VAL=%%x
        rmdir /S /Q "%PKG_FILE%\!VAL:/=\!"
    )
)

EXIT /B %ERRORLEVEL%

:WriteError
    echo "Requires at least four arguments (e.g. package.sh <package_name> <executable_name> <output_dir> <build_dir> [--env-vars | --excludes])"
EXIT /B 1

:loop
    if "%~1" NEQ "" (
        IF "%~1" EQU "--env-vars" ( IF "%~2" NEQ "--excludes" ( SET ENV_VARS=%~2 ) )
        IF "%~1" EQU "--excludes" ( SET EXCLUDES=%~2 )
        shift
        goto :loop
    )
EXIT /B 0

endlocal