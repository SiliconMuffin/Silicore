@echo off
setlocal

echo ============================================
echo   Building binary_explorer.exe
echo ============================================
echo.

rem --- find a compiler ---
set "CC="

where cl >nul 2>nul && set "CC=cl"
if not defined CC where gcc >nul 2>nul && set "CC=gcc"
if not defined CC where clang >nul 2>nul && set "CC=clang"
if not defined CC where tcc >nul 2>nul && set "CC=tcc"

if not defined CC (
    echo [ERROR] No C compiler found.
    echo.
    echo Install one of these:
    echo   1^) MSYS2 + mingw-w64  ^(https://www.msys2.org^)
    echo      then: pacman -S mingw-w64-x86_64-gcc
    echo.
    echo   2^) Visual Studio 2022 with "Desktop development with C++"
    echo      then run this .bat from "x64 Native Tools Command Prompt"
    echo.
    echo   3^) w64devkit ^(single zip, no install^): https://github.com/skeeto/w64devkit
    echo.
    pause
    exit /b 1
)

echo Using compiler: %CC%
echo.

if "%CC%"=="cl" (
    cl /nologo /O2 /W3 /Fe:binary_explorer.exe main.c
    if errorlevel 1 goto fail
    del /q main.obj 2>nul
) else (
    %CC% -O2 -Wall -o binary_explorer.exe main.c
    if errorlevel 1 goto fail
)

echo.
echo ============================================
echo   OK  ->  binary_explorer.exe
echo ============================================
echo.
echo Double-click binary_explorer.exe to play.
echo.
pause
exit /b 0

:fail
echo.
echo [ERROR] Build failed.
pause
exit /b 1
