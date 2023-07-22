@ECHO OFF

:: ===================================================================
:: Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
::
:: This code is released under an unmodified zlib license.
:: For conditions of distribution and use, please see:
::     https://opensource.org/licenses/Zlib
:: ===================================================================

SETLOCAL ENABLEDELAYEDEXPANSION

set argC=0
for %%x in (%*) do Set /A argC+=1

if %argC% LSS 1  CALL :WriteError && EXIT /B %ERRORLEVEL%

SET TARGET_FILE=%~1
SET CXXFLAGS=%~2
SET DIRS=%~3
SET DIRS=%DIRS:/=\%
SET /P FILE_CONTENTS=<%TARGET_FILE%

IF EXIST %TARGET_FILE% (
    IF "%FILE_CONTENTS%" EQU "%CXXFLAGS%" (
        FOR %%x IN (%DIRS%) DO (
            IF EXIST "%%x" rmdir /S /Q "%%x"
        )
    )
)

echo %CXXFLAGS% > %TARGET_FILE%

EXIT /B %ERRORLEVEL%

:WriteError
    echo "Requires at least two arguments (e.g. buildflags.bat <target_dir> <cxxflags> <dirs>)"
EXIT /B 1

ENDLOCAL