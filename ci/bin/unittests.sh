#!/usr/bin/env bash
CMAKE=cmake
CTEST=ctest
ScriptPath=$(dirname $(realpath $0))
BuildFolder=$(realpath ${ScriptPath}/../..)/build/$(uname)

echo === UnitTests
cd $BuildFolder
$CMAKE -DCMAKE_BUILD_TYPE=Debug ../..
$CMAKE --build .
$CTEST --verbose
cd -

$ScriptPath/py_unittests.sh
