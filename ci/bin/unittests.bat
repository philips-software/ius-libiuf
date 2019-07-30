@echo off
set BinFolder=%~dp0..\..\
pushd .
cd %BinFolder%
set BuildFolder=%CD%\build
mkdir %BuildFolder% & pushd %BuildFolder%
popd
echo === C library - UnitTests
set PlatformFolder=%BuildFolder%\Windows
mkdir %PlatformFolder% & pushd %PlatformFolder%
set Config=Debug
cmake -DCMAKE_CONFIG=%Config% ..\..
cmake --build . --config %Config%
if errorlevel 1 exit /B 1
ctest --verbose
if errorlevel 1 exit /B 1

set Config=Release
cmake -DCMAKE_CONFIG=%Config% ..\..
cmake --build . --config %Config%
if errorlevel 1 exit /B 1
ctest --verbose
if errorlevel 1 exit /B 1
echo === C library - UnitTests - Done

echo === Python binding - UnitTests
cmake --build . --target install-python
popd
python -m unittest discover python/tests
if errorlevel 1 exit /B 1
echo === Python binding - UnitTests - Done
