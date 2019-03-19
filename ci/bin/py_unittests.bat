echo off
set SdkRoot=%~dp0..\..\
set Python3Module=Python3Iuf.py
set Python2Module=Python2Iuf.py

set ModuleFolder=%SdkRoot%\build\Windows\python\python2
if exist "%ModuleFolder%\%Python2Module%" (
	set PYTHONPATH=%ModuleFolder%\Release;%ModuleFolder%;%PYTHONPATH%
)

set ModuleFolder=%SdkRoot%\build\Windows\python\python3
if exist "%ModuleFolder%\%Python3Module%" (
	set PYTHONPATH=%ModuleFolder%\Release;%ModuleFolder%;%PYTHONPATH%
)

set ModuleFolder=%SdkRoot%\python\python2\Windows
if exist "%ModuleFolder%\%Python2Module%" (
	set PYTHONPATH=%ModuleFolder%\Release;%ModuleFolder%;%PYTHONPATH%
)

set ModuleFolder=%SdkRoot%\python\python3\Windows
if exist "%ModuleFolder%\%Python3Module%" (
	set PYTHONPATH=%ModuleFolder%\Release;%ModuleFolder%;%PYTHONPATH%
)

echo === Python UnitTests
python -m unittest discover python\test
if errorlevel 1 exit /B 1

