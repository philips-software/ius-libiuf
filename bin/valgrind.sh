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
ValgrindMemReportFile=valgrind-report.xml
echo === Dynamic Code Analysis by Valgrind 
cd $BuildFolder
$CMAKE -DCMAKE_BUILD_TYPE=Debug ../..
make -j 4
valgrind --trace-children=yes --xml=yes --xml-file=$ValgrindMemReportFile unitTests/unitTests

