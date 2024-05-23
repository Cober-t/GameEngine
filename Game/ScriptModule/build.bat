@echo off

pushd %~dp0\..\..\
call libraries\premake\premake5.exe vs2022 --file=premake5.lua
popd


call "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat"

set solutionFile=ScriptingWorkspace.sln
msbuild /t:Build /p:Configuration=Debug /p:Platform=x64 %solutionFile%

xcopy /y ".\bin\ScriptModule\ScriptModule.dll" "..\..\bin\Debug-windows-x86_64\Editor"
xcopy /y ".\bin\ScriptModule\ScriptModule.dll" "..\build\Debug-windows-x86_64"