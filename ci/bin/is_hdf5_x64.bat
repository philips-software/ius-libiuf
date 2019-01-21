@echo off

:: Check suppported VS versions
if not "%VS140COMNTOOLS%" == "" (
    set DUMPBINPATH="%VS140COMNTOOLS:~0,-14%VC\bin\amd64\dumpbin.exe"
)

if not "%VS150COMNTOOLS%" == "" (
    set DUMPBINPATH="%VS150COMNTOOLS:~0,-14%VC\bin\amd64\dumpbin.exe"
)

set IS_X64=
set H5COMMAND_PATHNAME=
set TEMP_FILE=%TEMP%\is_hdf5_x64.txt
set H5COMMAND=h5copy.exe

:: if for some reason the TEMP_FILE exists
:: (from a previous run f.e.)s, remove it.
del %TEMP_FILE% 2>nul
where %H5COMMAND% > %TEMP_FILE% 2>nul
set /p H5COMMAND_PATHNAME=<%TEMP_FILE%
if "%H5COMMAND_PATHNAME%" == "" (
	echo Unable to find HDF55 distribution
) else (
	echo HDF5 lib architecture [%H5COMMAND_PATHNAME:\bin\h5copy.exe=%]:
	%DUMPBINPATH% /headers "%H5COMMAND_PATHNAME%"  | findstr machine
)

del %TEMP_FILE% 2>nul
