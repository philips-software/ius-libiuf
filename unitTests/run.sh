#!/bin/sh
Target=./unitTests

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$(pwd)/../lib:$(pwd)/../lib/hdf5/lib
echo LD_LIBRARY_PATH=$LD_LIBRARY_PATH

echo path is:
echo $LD_LIBRARY_PATH

make $Target
valgrind --tool=memcheck --leak-check=yes --track-origins=yes $Target

