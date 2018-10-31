#!/usr/bin/env bash
which cmake3 >/dev/null 2>&1
if (( $? == 0 ))
then
    # cmake3 found
    CMAKE=cmake3
    CTEST=ctest3
else
    # use cmake
    CMAKE=cmake
    CTEST=ctest
fi

BuildFolder=C_v3/build/$(uname)
CppCheckReportFile=cppcheck-report.xml

echo === Static code analysis for ius in $BuildFolder
mkdir -p $BuildFolder
cd $BuildFolder
$CMAKE -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ../..

cppcheck --enable=style --project=compile_commands.json  --xml-version=2 --xml 2> $CppCheckReportFile
