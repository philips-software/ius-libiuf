#!/usr/bin/env bash
CMAKE=cmake
CTEST=ctest
ScriptPath=$(dirname $(realpath $0))
BuildFolder=$(realpath ${ScriptPath}/../..)/build/$(uname)

echo === Building iuf in $BuildFolder
mkdir -p $BuildFolder
cd $BuildFolder
$CMAKE -DCMAKE_BUILD_TYPE=Release ../..
$CMAKE --build . --target install
$CMAKE -DCMAKE_BUILD_TYPE=Debug ../..
$CMAKE --build . --target install
