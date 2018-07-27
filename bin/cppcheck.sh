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
echo === Building ius in $BuildFolder
mkdir -p $BuildFolder
cd $BuildFolder
$CMAKE -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ../..

#cppcheck --error-exitcode=1 --enable=all --project=compile_commands.json  --xml 2> err.xml
cppcheck --enable=all --project=compile_commands.json  --xml 2> err.xml
cppcheck-htmlreport --file=err.xml --report-dir=test1 --source-dir=.
