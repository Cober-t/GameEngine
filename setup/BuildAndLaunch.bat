@echo off
setlocal

set "MODE=%~1"
set "PROJECT_PATH=%~2"
set "ROOT=C:\Users\Jorge\Documents\GameEngine"
set "BUILD_DIR=%ROOT%\build"

if "%MODE%"=="" goto :usage

cmake -S "%ROOT%" -B "%BUILD_DIR%"
if %ERRORLEVEL% neq 0 (
    echo CMake configuration failed!
    exit /b %ERRORLEVEL%
)

if /I "%MODE%"=="Editor" (
    cmake --build "%BUILD_DIR%" --target Editor --config Debug
    if %ERRORLEVEL% neq 0 (
        echo CMake build failed!
        exit /b %ERRORLEVEL%
    )

    call LaunchEditor.bat
    exit /b %ERRORLEVEL%
)

if /I "%MODE%"=="Game" (
    if "%PROJECT_PATH%"=="" (
        echo Game mode requires a project path.
        echo Example: BuildAndLaunch.bat Game "C:\path\to\project"
        exit /b 1
    )

    cmake --build "%BUILD_DIR%" --target Game --config Debug
    if %ERRORLEVEL% neq 0 (
        echo CMake build failed!
        exit /b %ERRORLEVEL%
    )

    call LaunchGame.bat "%PROJECT_PATH%"
    exit /b %ERRORLEVEL%
)

echo Invalid mode: %MODE%
goto :usage

:usage
echo Usage:
echo   BuildAndLaunch.bat Editor
echo   BuildAndLaunch.bat Game "C:\path\to\your_project"
exit /b 1