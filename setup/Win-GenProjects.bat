@echo off
pushd %~dp0\..\
call libraries\premake\premake5.exe vs2022
popd
