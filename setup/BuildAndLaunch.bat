@echo off
setlocal

set "MODE=%~1"
set "PROJECT_PATH=%~2"
set "ROOT=C:\Users\JorgePC\Documents\GameEngine"
set "BUILD_DIR=%ROOT%\build"

if "%MODE%"=="" goto :usage

cmake -S "%ROOT%" -B "%BUILD_DIR%"
if errorlevel 1 (
    echo CMake configuration failed!
    exit /b 1
)

if /I "%MODE%"=="Editor" (
    cmake --build "%BUILD_DIR%" --target Editor --config Debug
    if errorlevel 1 (
        echo CMake build failed!
        exit /b 1
    )

    pushd "%~dp0\..\Editor"
    "%~dp0\..\build\bin\Debug\Editor.exe"
    popd
    exit /b %errorlevel%
)

if /I "%MODE%"=="Game" (
    if "%PROJECT_PATH%"=="" (
        echo Game mode requires a project path.
        echo Example: BuildAndLaunch.bat Game "C:\path\to\project"
        exit /b 1
    )

    cmake --build "%BUILD_DIR%" --target Game --config Debug
    if errorlevel 1 (
        echo CMake build failed!
        exit /b 1
    )

    "%~dp0\..\build\bin\Debug\Game.exe" --project "%PROJECT_PATH%"
    exit /b %errorlevel%
)

echo Invalid mode: %MODE%
goto :usage

:usage
echo Usage:
echo   BuildAndLaunch.bat Editor
echo   BuildAndLaunch.bat Game "C:\path\to\your_project"
exit /b 1