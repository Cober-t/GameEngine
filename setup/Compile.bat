@echo off

set SOLUTION_FILE="../Cober.sln"
set VS_PATH = "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7"
set OPEN_VS =  %VS_PATH%"\IDE\devenv.exe"
set COMPILE_VS_TOOL = %VS_PATH%"\Tools\VsDevCmd.bat"

REM Start "" %VS_PATH%  "."

python Setup.py
@REM cd bin
@REM cmake ../

call %COMPILE_VS_TOOL%

msbuild /t:Build /p:Configuration=Debug /p:Platform=x64 %SOLUTION_FILE%
