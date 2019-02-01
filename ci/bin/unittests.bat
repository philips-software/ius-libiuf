@echo off
set BinFolder=%~dp0..\..\
pushd .
cd %BinFolder%
set BuildFolder=%CD%\build
popd
echo === Building ius in %BuildFolder%
set PlatformFolder=%BuildFolder%\Windows
mkdir %PlatformFolder% & pushd %PlatformFolder%
cmake ..\..
cmake --build . --config Debug
if errorlevel 1 exit /B 1
ctest --verbose
if errorlevel 1 exit /B 1
popd