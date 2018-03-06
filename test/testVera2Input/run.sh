#!/bin/sh

echo LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${PWD}/../../lib
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${PWD}/../../lib:../../lib/hdf5/lib

echo path is:
echo $LD_LIBRARY_PATH

make

#valgrind --tool=memcheck --leak-check=yes --track-origins=yes ./testVera2Input
#testCWC4DWD

INPUT_FOLDER=/Users/nlv09165/proj/PPF/ius/INPUT
LD_INPUTFILE=$INPUT_FOLDER/regression/Vera2Input/example.mat
LD_OUTPUTFILE=$INPUT_FOLDER/regression/Vera2Input/example_result
LD_LOGFILE=$INPUT_FOLDER/regression/Vera2Input/example.log
./testVera2Input $LD_OUTPUTFILE $LD_INPUTFILE  $LD_LOGFILE

