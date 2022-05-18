@ECHO OFF

set PKG_NAME=%~1
set OUTPUT_DIR=%~2
set BIN_DIR=%~3
set DEBUG=%~4
set PKG_FILE="%OUTPUT_DIR%/%PKG_NAME%"

mkdir %PKG_FILE%
robocopy "%BIN_DIR%/lib" "%PKG_FILE%" * /E
robocopy "%BIN_DIR%/shaders" "%PKG_FILE%/shaders" * /E
robocopy %BIN_DIR% %PKG_FILE% app.exe
rename "%PKG_FILE%/app.exe" "%PKG_NAME%.exe"
