#!/usr/bin/env bash
CMAKE=cmake
CTEST=ctest
ScriptPath=$(dirname $(realpath $0))
BuildFolder=$(realpath ${ScriptPath}/../..)/build/$(uname)

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

