#!/bin/sh

cd ..
make
cd unitTests

Target=./unitTests

export LD_LIBRARY_PATH=$(pwd)/../lib:$(pwd)/../lib/hdf5/lib:$LD_LIBRARY_PATH
echo LD_LIBRARY_PATH=$LD_LIBRARY_PATH

echo path is:
echo $LD_LIBRARY_PATH

make $Target
if [[ $1 = valgrind ]] 
then
	valgrind --tool=memcheck --leak-check=yes --track-origins=yes $Target
else
	$Target
fi

