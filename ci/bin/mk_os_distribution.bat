@echo off
set BinFolder=%~dp0..\..\
pushd .
cd %BinFolder%
set BuildFolder=%CD%\build
mkdir %BuildFolder% & pushd %BuildFolder%
popd
echo === Building iuf in %BuildFolder%
set PlatformFolder=%BuildFolder%\Windows
mkdir %PlatformFolder% & pushd %PlatformFolder%
set Config=Debug
cmake -DCMAKE_CONFIG=%Config% ..\..
if errorlevel 1 exit /B 1
cmake --build .  --target install  --config %Config%
if errorlevel 1 exit /B 1
set Config=Release
cmake -DCMAKE_CONFIG=%Config% ..\..
if errorlevel 1 exit /B 1
cmake --build .  --target install  --config %Config%
if errorlevel 1 exit /B 1
popd