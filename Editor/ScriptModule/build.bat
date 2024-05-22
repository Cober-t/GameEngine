@echo off

@REM IF "%~1" == "generate" GOTO GenerateProject
@REM IF "%~1" == "compile" GOTO Compile
@REM IF "%~1" == "" GOTO Compile

@REM :GenerateProject
REM Build the project files
C:\Users\Jorge\Documents\GameEngine\libraries\premake\premake5.exe vs2022 --file=premake5.lua
@REM GOTO Done

@REM :Compile

call "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat"

set solutionFile=ScriptingWorkspace.sln
msbuild /t:Build /p:Configuration=Debug /p:Platform=x64 %solutionFile%

xcopy /y ".\bin\ScriptModule\ScriptModule.dll" "..\..\bin\Debug-windows-x86_64\Editor"
xcopy /y ".\bin\ScriptModule\ScriptModule.dll" "..\build\Debug-windows-x86_64"

@REM :Done