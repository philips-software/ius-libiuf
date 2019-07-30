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

echo === C library - UnitTests

cd $BuildFolder
$CMAKE -DCMAKE_BUILD_TYPE=Release ../..
$CTEST --verbose
echo === C library - UnitTests - Done

echo === Python binding - UnitTests
$CMAKE --build . --target install-python
cd -
python -m unittest discover python/tests
echo === Python binding - UnitTests - Done
