@echo off
set BinFolder=%~dp0..\
pushd .
cd %BinFolder%
set BuildFolder=%CD%\build\Windows
popd
echo === Building ius in %BuildFolder%
mkdir %BuildFolder% & pushd %BuildFolder%
cmake ..\..
cmake --build . --config Debug
ctest --verbose
popd