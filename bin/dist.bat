@echo off
set BinFolder=%~dp0..\
pushd .
cd %BinFolder%
set BuildFolder=%CD%\build\Windows
popd
echo === Building ius in %BuildFolder%
mkdir %BuildFolder% & pushd %BuildFolder%
cmake ..\..
cmake --build . --target install --config Debug
cmake --build . --target install --config Release
popd