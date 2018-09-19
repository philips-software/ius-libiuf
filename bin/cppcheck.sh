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
CppCheckReportFile=cppcheck-report.xml

echo === Static code analysis for ius in $BuildFolder
mkdir -p $BuildFolder
cd $BuildFolder
$CMAKE -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ../..

cppcheck --enable=style --project=compile_commands.json  --xml-version=2 --xml 2> $CppCheckReportFile
