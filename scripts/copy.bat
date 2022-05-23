@echo off

setlocal enableextensions

set FIRST_ARG=%1
set FIRST_ARG=%FIRST_ARG:/=\%
set DESTINATION=%2
set DESTINATION=%DESTINATION:/=\%
set FILE=%3
set FILE=%FILE:/=\%

if "%1" EQU "--copy-directory" ( CALL :XCopy %DESTINATION% %FILE% ) ELSE ( CALL :Robocopy %FIRST_ARG% %DESTINATION% %FILE% )

EXIT /B %ERRORLEVEL%

endlocal 

:XCopy
    XCopy "%~1" "%~2" /s /i
EXIT /B 0

:Robocopy 
    robocopy "%~1" "%~2" "%~3"
EXIT /B 0