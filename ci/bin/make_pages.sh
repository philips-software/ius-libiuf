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

Branch=$(git rev-parse --abbrev-ref HEAD)
ScriptPath=$(dirname $(realpath $0))
BuildFolder=$(realpath ${ScriptPath}/../..)/build
StartFolder=$(pwd)
DocSource="${StartFolder}/dox"


if [[ "$Branch" == "develop" || "$Branch" == "master" ]]
then
    DocFolder="${StartFolder}/public/${Branch}"
else
    DocFolder="${StartFolder}/public"
fi

echo === Generating documentation

export PROJECT_NUMBER=$(releaseNumber)
cd ${DocSource}
doxygen 2>&1 | grep -iv Warning
if [[ -d html ]]
then
    mv html ${DocFolder}
fi
echo === Done
