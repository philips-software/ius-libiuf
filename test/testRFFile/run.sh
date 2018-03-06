#!/bin/sh

echo LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${PWD}/../../lib
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${PWD}/../..

echo path is:
echo $LD_LIBRARY_PATH

make -f Makefile.h5cc

./testRFFile
#valgrind --tool=memcheck --leak-check=yes --track-origins=yes ./testRFFile
#testCWC4DWD

