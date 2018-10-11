@echo off
set BuildFolder=C_v3\build\WindowsX64
echo === Building ius in %BuildFolder%
mkdir %BuildFolder% & pushd %BuildFolder%
ctest --verbose
popd