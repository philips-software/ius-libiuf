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

ScriptPath=$(dirname $(realpath $0))
BuildFolder=$(realpath ${ScriptPath}/../..)/build
StartFolder=$(pwd)
DistFolder=${StartFolder}/dist
IUFRelease="IUF-SDK-$(releaseNumber)"
ReleaseFolder="${DistFolder}/${IUFRelease}"
DocFolder="${ReleaseFolder}/documentation"
DocSource="${StartFolder}/dox"



# merge dist folders into one
echo === Merging dist in $BuildFolder
[[ ! -d ${BuildFolder} ]] && echo "Error: No distribution data found in ${BuildFolder}" >&2 && exit 1

#
# because this step involves merging files coming
# from both Windows and Linux based, it is important
# to restore the executable permissions for
# the binaries..
find . -type f -name \*.sh | xargs chmod 755

for i in ${BuildFolder}/*
do
    echo " - ${i##*/}"
    [[ ! -d $i/dist ]] && echo "Error: Incomplete distribution for platform ${i##*/}" >&2 && exit 1
    (cd $i;tar cfz dist.tgz dist)
    tar xfz $i/dist.tgz
done

echo === Creating release package ${IUFRelease}
[[ ! -d ${DistFolder} ]] && echo "Error: No distribution data found" >&2 && exit 1
[[ -d ${ReleaseFolder} ]] && rm -rf ${ReleaseFolder}

mkdir -p ${ReleaseFolder}
mv ${DistFolder}/* ${ReleaseFolder} 2>/dev/null

echo === Generating documentation
export PROJECT_NUMBER=$(releaseNumber)
mkdir -p ${DocFolder}
cd ${DocSource}
doxygen 2>&1 | grep -iv Warning
if [[ -d html ]]
then
    tar cfz - html | (cd ${DocFolder}; tar xfz -)
fi
echo === Done
