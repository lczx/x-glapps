@echo off

:: Download dependencies
call :dl "freeglut-2.8.1" "freeglut" "http://files.transmissionzero.co.uk/software/development/GLUT/freeglut-MSVC.zip"
call :dl "glew-1.12.0" "glew-1.12.0" "http://sourceforge.net/projects/glew/files/glew/1.12.0/glew-1.12.0-win32.zip"
call :dl "glm-0.9.6.1" "glm" "http://sourceforge.net/projects/ogl-math/files/glm-0.9.6.1/glm-0.9.6.1.zip"
call :dl "soil-july2008" "Simple OpenGL Image Library" "http://www.lonesock.net/files/soil.zip"

:: Make VS happy
if not exist "freeglut-2.8.1/bin/Win32" (
    pushd "freeglut-2.8.1"
    md "bin/Win32" "lib/Win32"
    move "bin\freeglut.dll" "bin/Win32"
    move "lib\freeglut.lib" "lib/Win32"
    popd
)

:: Add precompiled SOIL binaries for Windows
if not exist "soil-july2008/lib/Debug-x64" tar -xvf soil-binaries-win.tgz

goto :EOF


:: Incremental download + unzip proc (new_name, old_name, url)
:dl
if not exist %1 (
    echo.
    echo ------------------------- Downloading %1 -------------------------
    wget -O tmp %3
    unzip tmp
    del tmp
    move %2 %1
)
goto :EOF
