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

echo === UnitTests
cd $BuildFolder
$CMAKE -DCMAKE_BUILD_TYPE=Debug ../..
$CMAKE --build .
$CTEST --verbose
cd -

$ScriptPath/py_unittests.sh
