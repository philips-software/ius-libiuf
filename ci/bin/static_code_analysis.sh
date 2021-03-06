#!/usr/bin/env bash
ScriptPath=$(dirname $(realpath $0))
BuildFolder=$(realpath ${ScriptPath}/../..)/build/$(uname)
CppCheckReportFile=cppcheck-report.xml

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

echo === Static code analysis for iuf in $BuildFolder
mkdir -p $BuildFolder
cd $BuildFolder
$CMAKE -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ../..

cppcheck -j ${CMAKE_BUILD_PARALLEL_LEVEL:=1} --enable=style --project=compile_commands.json  --xml-version=2 --xml 2> $CppCheckReportFile
