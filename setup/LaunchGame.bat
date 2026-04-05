@echo off
pushd %~dp0\..\build\bin\Debug
call "Game.exe" "--project" %1
popd
