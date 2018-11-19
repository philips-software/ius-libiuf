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
echo === Building ius in $BuildFolder
mkdir -p $BuildFolder
cd $BuildFolder
$CMAKE -DCMAKE_BUILD_TYPE=Debug ../..
$CMAKE --build . --target install
$CMAKE -DCMAKE_BUILD_TYPE=Release ../..
$CMAKE --build . --target install
