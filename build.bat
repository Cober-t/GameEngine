@echo off
setlocal enabledelayedexpansion

:: Build script for Cober Game Engine
echo Building Cober Game Engine...

:: Check for AddressSanitizer flag
set ASAN_FLAG=OFF
if /i "%1"=="asan" (
    set ASAN_FLAG=ON
    shift
)

:: Check if specific game project is requested
if "%1"=="game" (
    if "%2"=="" (
        echo Error: Game name required when using 'game' option
        echo Usage: build.bat [asan] game [GameName] [configuration]
        exit /b 1
    )
    set GAME_NAME=%2
    set CONFIG=%3
    if "!CONFIG!"=="" set CONFIG=Debug
    
    echo Building game project: !GAME_NAME! in !CONFIG! configuration
    if "!ASAN_FLAG!"=="ON" echo AddressSanitizer: ENABLED
    
    cmake --preset game-debug -DGAME_PROJECT=!GAME_NAME! -DCMAKE_BUILD_TYPE=!CONFIG! -DENABLE_ASAN=!ASAN_FLAG!
    cmake --build build-game --config !CONFIG!
) else (
    :: Default build - Editor and Engine
    set CONFIG=%1
    if "!CONFIG!"=="" set CONFIG=Debug
    
    echo Building Editor and Engine in !CONFIG! configuration
    if "!ASAN_FLAG!"=="ON" echo AddressSanitizer: ENABLED
    
    if /i "!CONFIG!"=="Debug" (
        if "!ASAN_FLAG!"=="ON" (
            cmake --preset windows-x64-debug-asan
            cmake --build build-asan --config Debug
        ) else (
            cmake --preset windows-x64-debug
            cmake --build build --config Debug
        )
    ) else if /i "!CONFIG!"=="Release" (
        if "!ASAN_FLAG!"=="ON" (
            cmake --preset windows-x64-release-asan
            cmake --build build-asan --config Release
        ) else (
            cmake --preset windows-x64-release
            cmake --build build --config Release
        )
    ) else if /i "!CONFIG!"=="Dist" (
        if "!ASAN_FLAG!"=="ON" (
            echo Warning: AddressSanitizer not recommended for Distribution builds
        )
        cmake --preset windows-x64-dist
        cmake --build build --config Dist
    ) else (
        echo Invalid configuration: !CONFIG!
        echo Valid configurations: Debug, Release, Dist
        echo Usage: build.bat [asan] [configuration]
        echo        build.bat [asan] game [GameName] [configuration]
        exit /b 1
    )
)

echo Build completed!
echo.
if "!ASAN_FLAG!"=="ON" (
    echo ======================================
    echo   AddressSanitizer is ENABLED
    echo ======================================
    echo.
    echo Your application will run slower but will detect:
    echo - Buffer overflows/underflows
    echo - Use-after-free errors
    echo - Memory leaks
    echo - Double-free errors
    echo.
    echo For best results with MSVC, make sure you have:
    echo - Visual Studio 2019 16.9+ or Visual Studio 2022
    echo - Windows SDK 10.0.20348.0 or later
    echo.
)
pause