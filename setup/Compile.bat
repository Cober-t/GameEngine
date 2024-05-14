@echo off

set VS_PATH = "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7"
set OPEN_VS =  %VS_PATH%"\IDE\devenv.exe"
set COMPILE_VS_TOOL = %VS_PATH%"\Tools\VsDevCmd.bat"

python Setup.py %*

call %COMPILE_VS_TOOL%

set SOLUTION_FILE="../Cober.sln"
msbuild /t:Build /p:Configuration=Debug /p:Platform=x64 %SOLUTION_FILE%
