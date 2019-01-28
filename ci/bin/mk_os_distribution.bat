@echo off
set BinFolder=%~dp0..\..\
pushd .
cd %BinFolder%
set BuildFolder=%CD%\build\Windows
popd
echo === Building ius in %BuildFolder%
mkdir %BuildFolder% & pushd %BuildFolder%
cmake ..\..
cmake --build . --target install --config Debug
if errorlevel 1 exit 1
cmake --build . --target install --config Release
if errorlevel 1 exit 1
popd