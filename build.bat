@echo off
:: Example: build.bat engine debug asan
:: Build script for Cober Game Engine
echo Building Cober Game Engine...

:: Check for AddressSanitizer flag
set ASAN_FLAG=OFF
set PROJECT=%1
set CONFIG=%2
set OPT=%3
set PRESET_NAME="windows-x64-editor"
set BUILD_NAME="build-editor"

:: Defaults
if "%PROJECT%"=="" set PROJECT = Editor 
if "%CONFIG%" =="" set CONFIG  = Debug

if /i "%CONFIG%"=="debug" if "%OPT%"=="asan" (
    echo ======================================
    echo   AddressSanitizer is ENABLED
    echo ======================================
    set ASAN_FLAG=ON
    shift
)

:: Check if specific game project is requested
if "%PROJECT%"=="game" if "%OPT%" == "asan" set OPT = "Game"

if "%ASAN_FLAG%"=="ON" (
    set PRESET_NAME="windows-x64-%CONFIG%-asan"
    set BUILD_NAME="build-%PROJECT%-asan"
) else (
    set PRESET_NAME="windows-x64-%CONFIG%"
    set BUILD_NAME="build-%PROJECT%"
)

set BINARY_DIR=%~dp0build\%BUILD_NAME%
mkdir %BINARY_DIR%

:: Building
echo Building: %PROJECT% in %CONFIG% configuration
if "%ASAN_FLAG%"=="ON" echo AddressSanitizer: ENABLED

cmake -B ./build/build-%PROJECT% -DPROJECT=%PROJECT% -DGAME_NAME=%OPT% -DBUILD_TYPE=%CONFIG% -DBUILD_DIR=%BUILD_NAME% -DENABLE_ASAN=%ASAN_FLAG% -DCONFIG=%CONFIG%


echo Build completed!


:: Run application
@REM cmd /c start C:/Users/Jorge/Development/GameEngine/build/bin/Debug/Editor/Editor.exe