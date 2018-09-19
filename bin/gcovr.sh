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
CppCheckReportFile=gcovr-report.xml

echo === Generating code coverage for unit tests
mkdir -p $BuildFolder
cd $BuildFolder
$CMAKE -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ../..
make gcovr-report
