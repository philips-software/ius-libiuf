#!/usr/bin/env bash

function releaseNumber
{
    # tag found? use that
    CommitHash=$(git log -1 --format=%h)
    GitTag=$(git describe --tags --abbrev=0 --exact-match 2> /dev/null)
    if (( $? == 0 ))
    then
        # otherwise use commit hash
        Release="3.${GitTag}"
    else
        # otherwise use commit hash
        Release="3.${CommitHash}"
    fi
    echo -n "$Release"
}

BuildFolder=C_v3/build
StartFolder=$(pwd)
DistFolder=${StartFolder}/dist
IUSRelease="ius-SDK-$(releaseNumber)"
ReleaseFolder="${DistFolder}/${IUSRelease}"
DocFolder="${ReleaseFolder}/documentation"
DocSource="${StartFolder}/C_v3/dox"

# merge dist folders into one
echo === Merging dist in $BuildFolder
for i in ${BuildFolder}/*
do
    echo " - ${i##*/}"
    [[ ! -d $i/dist ]] && echo "Error: Incomplete distribution for platform ${i##*/}" >&2 && exit 1
    (cd $i; tar cfz dist.tgz dist)
    tar xfz $i/dist.tgz
done



echo === Creating release package ${IUSRelease}
cd ${DistFolder}
[[ -d ${ReleaseFolder} ]] && rm -rf ${ReleaseFolder}
mkdir -p ${DocFolder}
mv * ${ReleaseFolder} 2>/dev/null

echo === Generating documentation
cd ${DocSource}
doxygen 2>&1 | grep -iv Warning
if [[ -d html ]]
then
    mv html ${DocFolder}
    cp -r img ${DocFolder}
fi
echo === Done
