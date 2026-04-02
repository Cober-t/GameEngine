@echo off

cmake -S C:\Users\Jorge\Documents\GameEngine -B C:\Users\Jorge\Documents\GameEngine\build
cmake --build C:\Users\Jorge\Documents\GameEngine\build --config Debug

call LaunchEditor.bat


@REM set VS_PATH = "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7"
@REM set OPEN_VS =  %VS_PATH%"\IDE\devenv.exe"
@REM set COMPILE_VS_TOOL = %VS_PATH%"\Tools\VsDevCmd.bat"

@REM python %~dp0Setup.py %*

@REM call %COMPILE_VS_TOOL%

@REM set SOLUTION_FILE=%~dp0..\Cober.sln
@REM msbuild /t:Build /p:Configuration=Debug /p:Platform=x64 %SOLUTION_FILE%
