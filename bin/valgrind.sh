#!/usr/bin/env bash
which cmake >/dev/null 2>&1
if (( $? == 0 ))
then
    # cmake found
    CMAKE=cmake
    CTEST=ctest
else
    # use cmake3
    CMAKE=cmake3
    CTEST=ctest3
fi
BuildFolder=C_v3/build/$(uname)

if [[ $1 = "xml" ]]
then
	ValgrindMemReportFile=valgrind-report.xml
	ValgrindOpts="--xml=yes --xml-file=$ValgrindMemReportFile"
else
	ValgrindOpts="--leak-check=full"
fi

echo === Dynamic Code Analysis by Valgrind 
cd $BuildFolder
$CMAKE -DCMAKE_BUILD_TYPE=Debug ../..
make -j 4
valgrind --trace-children=yes $ValgrindOpts unitTests/unitTests

