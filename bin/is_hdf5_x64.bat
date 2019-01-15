@echo off
set TEMP_FILE=%RANDOM%.txt
set H5COMMAND=h5copy
where %H5COMMAND% > %TEMP_FILE%
set /p H5COMMAND_PATHNAME=<%TEMP_FILE%
dumpbin /headers %H5COMMAND_PATHNAME%