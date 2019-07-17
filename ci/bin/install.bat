@echo on
set DownloadFolder=%~dp0..\..\Download
pushd .

echo === Downloading HDF5
mkdir %DownloadFolder%
cd %DownloadFolder%
curl -L -0 -J https://support.hdfgroup.org/ftp/HDF5/releases/hdf5-1.8/hdf5-1.8.20/bin/windows/hdf5-1.8.20-Std-win7_64-vs14.zip -o ./hdf5-1.8.20-Std-win7_64-vs14.zip
mkdir HDF5
7z x ./hdf5-1.8.20-Std-win7_64-vs14.zip -o./HDF5
dir HDF5


if errorlevel 1 exit /B 1
popd
