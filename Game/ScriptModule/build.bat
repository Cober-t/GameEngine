@echo off

IF "%~1" == "generate" GOTO GenerateProject
IF "%~1" == "compile" GOTO Compile

:GenerateProject
REM Build the project files
C:\Users\Jorge\Documents\GameEngine\libraries\premake\premake5.exe vs2022 --file=premake5.lua
GOTO Done

:Compile

C:\Users\Jorge\Documents\GameEngine\libraries\premake\premake5.exe vs2022

call "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat"

set solutionFile=ScriptingWorkspace.sln
msbuild /t:Build /p:Configuration=Debug /p:Platform=x64 %solutionFile%

:Done