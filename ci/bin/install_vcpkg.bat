@echo off
set DownloadFolder=%cd%\Download
set VcpkgFolder=%DownloadFolder%\vcpkg

pushd .

:: Check if Download folder exists
pushd %DownloadFolder% && popd || mkdir %DownloadFolder%
cd %DownloadFolder%

if not exist %VcpkgFolder% (
    echo === Installing VCPKG..
    git clone https://github.com/Microsoft/vcpkg.git
    cd vcpkg
    .\bootstrap-vcpkg.bat
    echo Done
) else (
    echo  VCPKG has already been installed.
    cd vcpkg
)


echo === Installing library hdf5:x64-windows-static..
:: .\vcpkg list
:: .\vcpkg remove hdf5:x64-windows-static
.\vcpkg install hdf5:x64-windows-static
:: .\vcpkg list
echo Done
if errorlevel 1 exit /B 1
popd
