@echo off
pushd %~dp0\..\
call bin\Debug-windows-x86_64\Editor\Editor.exe %*
popd
