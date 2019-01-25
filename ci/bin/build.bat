@echo off
set BinFolder=%~dp0..\..\
pushd .
cd %BinFolder%
set BuildFolder=%CD%\build\Windows
popd
echo === Building ius in %BuildFolder%
mkdir %BuildFolder% & pushd %BuildFolder%
cmake ..\..
cmake --build . --config Release
echo cmake --build . --config Release: %ERRORLEVEL%
if errorlevel 1 exit /B
cmake --build . --config Debug
echo cmake --build . --config Debug  %ERRORLEVEL%
if errorlevel 1 exit /B
popd
