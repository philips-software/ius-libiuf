@echo off
set SdkRoot=%~dp0..\..\
set Python3Module=Python3Ius.py
set Python2Module=Python2Ius.py


set ModuleFolder=%SdkRoot%\build\Windows\python\python2
if exist "%ModuleFolder%\%Python2Module%" (
	set PYTHONPATH=%ModuleFolder%;%PYTHONPATH%
)

set ModuleFolder=%SdkRoot%\build\Windows\python\python3
if exist "%ModuleFolder%\%Python3Module%" (
	set PYTHONPATH=%ModuleFolder%;%PYTHONPATH%
)

set ModuleFolder=%SdkRoot%\python\python2\Windows
if exist "%ModuleFolder%\%Python2Module%" (
	set PYTHONPATH=%ModuleFolder%;%PYTHONPATH%
)

set ModuleFolder=%SdkRoot%\python\python3\Windows
if exist "%ModuleFolder%\%Python3Module%" (
	set PYTHONPATH=%ModuleFolder%;%PYTHONPATH%
)

echo === Python UnitTests
python -m unittest discover python\test
if errorlevel 1 exit /B 1

