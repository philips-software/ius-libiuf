#!/usr/bin/env bash

BuildFolder=C_v3/build

# merge dist folders into one
echo === Merging dist in $BuildFolder
for i in ${BuildFolder}/*
do
    [[ ! -d $i/dist ]] && echo "Error: Incomplete distribution for platform ${i##*/}" >&2 && exit 1
    (cd $i; tar cvfz dist.tgz dist)
    (cd $BuildFolder; tar xvfz ${i##*/}/dist.tgz)
    rm -rf $i
done
echo === Merging dist in $BuildFolder Done
exit 0