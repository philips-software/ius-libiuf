#!/usr/bin/env bash
BuildFolder=C_v3/build/$(uname)
echo === Building ius in $BuildFolder
mkdir -p $BuildFolder
cd $BuildFolder
cmake3 ../..
exec make
