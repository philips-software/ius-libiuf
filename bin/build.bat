@echo off
set BuildFolder=C_v3\build\WindowsX64
echo === Building ius in %BuildFolder%
mkdir %BuildFolder% & pushd %BuildFolder%
cmake -G "Visual Studio 15 2017 Win64" --config Release ..\..
cmake --build .
popd