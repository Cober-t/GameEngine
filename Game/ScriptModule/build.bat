@echo off

call %~pd0..\..\libraries\premake\premake5.exe vs2022 --file=%~pd0premake5.lua

call "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat"

msbuild /t:Build /p:Configuration=Debug /p:Platform=x64 %~pd0ScriptingWorkspace.sln

xcopy /y "%~pd0bin\ScriptModule\ScriptModule.dll" "%~pd0..\..\bin\Debug-windows-x86_64\Editor"
xcopy /y "%~pd0bin\ScriptModule\ScriptModule.dll" "%~pd0..\build\Debug-windows-x86_64"