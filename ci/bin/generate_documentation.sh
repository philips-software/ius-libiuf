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
IUSRelease="IUS-SDK-$(releaseNumber)"
DocFolder="${StartFolder}/public/${IUSRelease}/documentation"
DocFolder="${StartFolder}/public"
DocSource="${StartFolder}/dox"

echo === Generating documentation

export PROJECT_NUMBER=$(releaseNumber)
mkdir -p ${DocFolder}
cd ${DocSource}
doxygen 2>&1 | grep -iv Warning
if [[ -d html ]]
then
    mv html ${DocFolder}
    cp -r img ${DocFolder}
fi
echo === Done
