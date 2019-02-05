@echo off
set BinFolder=%~dp0..\..\
pushd .
cd %BinFolder%
set BuildFolder=%CD%\build
popd
echo === Building ius in %BuildFolder%
set PlatformFolder=%BuildFolder%\Windows
mkdir %PlatformFolder% & pushd %PlatformFolder%
set Config=Debug
cmake -DCMAKE_BUILD_TYPE=%Config% ..\..
cmake --build . --config %Config%
if errorlevel 1 exit /B 1
set Config=Release
cmake -DCMAKE_BUILD_TYPE=%Config% ..\..
cmake --build . --config %Config%
if errorlevel 1 exit /B 1
popd