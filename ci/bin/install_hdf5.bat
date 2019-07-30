@echo off
set DownloadFolder=%~f1
set DownloadLocation="https://support.hdfgroup.org/ftp/HDF5/releases/hdf5-1.8/hdf5-1.8.20/bin/windows"
set ZipFile="hdf5-1.8.20-Std-win7_64-vs14.zip"
set DownloadURL="%DownloadLocation%/%ZipFile%"
set Hdf5File=HDF5-1.8.20-win64.msi
set Hdf5Base=%Hdf5file:~0,-4%
set Hdf5InstallFolder=%DownloadFolder%
set Hdf5InstallLogFile=%Hdf5InstallFolder%\msiexec_install.log.txt
echo %Hdf5InstallFolder%

pushd .
:: Check if Download folder exists
pushd %DownloadFolder% && popd || mkdir %DownloadFolder%
cd %DownloadFolder%

if not exist %ZipFile% (
    echo === Downloading HDF5..
    curl -L -0 -J %DownloadURL% -o %ZipFile%
    echo Done
) else (
    echo  %ZipFile% has already been downloaded.
)

if not exist %Hdf5File% (
    echo === Extracting HDF5..
    7z e %ZipFile% *.msi -r
    echo Done
) else (
    echo  %Hdf5File% has already been extracted.
)

if not exist %Hdf5InstallLogFile% (
    echo === Installing HDF5..
    msiexec /i %Hdf5File% INSTALL_ROOT=%Hdf5InstallFolder% /qn /L*v %Hdf5InstallLogFile%
    echo Done
) else (
    echo  HDF5 librarry has already been installed.
)


dir %Hdf5InstallFolder%
if errorlevel 1 exit /B 1
popd
