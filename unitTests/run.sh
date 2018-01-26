#!/bin/sh

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${PWD}/../lib
echo LD_LIBRARY_PATH=$LD_LIBRARY_PATH

echo path is:
echo $LD_LIBRARY_PATH

make

./unitTests

