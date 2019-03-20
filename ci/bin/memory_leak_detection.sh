#!/usr/bin/env bash
ScriptPath=$(dirname $(realpath $0))
BuildFolder=$(realpath ${ScriptPath}/../..)/build/$(uname)

#which cmake3 >/dev/null 2>&1
#if (( $? == 0 ))
#then
#    # cmake3 found
#    CMAKE=cmake3
#    CTEST=ctest3
#else
#    # use cmake
#    CMAKE=cmake
#    CTEST=ctest
#fi

CMAKE=cmake
CTEST=ctest

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

