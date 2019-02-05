#!/usr/bin/env bash
ScriptPath=$(dirname $(realpath $0))
BuildFolder=$(realpath ${ScriptPath}/../..)/build/$(uname)

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

echo === Building ius in $BuildFolder
mkdir -p $BuildFolder
cd $BuildFolder
$CMAKE -DCMAKE_BUILD_TYPE=Debug ../..
$CMAKE --build . --target install
$CMAKE -DCMAKE_BUILD_TYPE=Release ../..
$CMAKE --build . --target install
