#!/usr/bin/env bash
CMAKE=cmake
CTEST=ctest
ScriptPath=$(dirname $(realpath $0))
BuildFolder=$(realpath ${ScriptPath}/../..)/build/$(uname)
CppCheckReportFile=gcovr-report.xml

echo === Generating code coverage for unit tests
mkdir -p $BuildFolder
cd $BuildFolder
$CMAKE -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ../..
make gcovr-report
