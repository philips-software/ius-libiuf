#!/bin/bash

function figho
{
    echo $* | figlet
}


function tmate_remote_debug
#
# Call this function when in need of debugging aids.
# This routine allows you to ssh into the build session
# and expect environment variables and such.
# (It is using tmate.io tunneling services)
#
{
    figho "Tmate session.."
    echo  | ssh-keygen -t rsa -N ""
    tmate -S /tmp/tmate.sock new-session -d               # Launch tmate in a detached state
    tmate -S /tmp/tmate.sock wait tmate-ready             # Blocks until the SSH connection is established
    tmate -S /tmp/tmate.sock display -p '#{tmate_ssh}'    # Prints the SSH connection string
    while true
    do
        printf "."
        TMATEPID=$(ps -eaf | awk '/.*tmate \-S.* -d/ { print $2 }')
        if [[ $TMATEPID == "" ]]
        then
            break
        fi
        sleep 2
    done
    figho "Tmate closed.."
}


#-----------------------------
# L I N U X - C I
#-----------------------------
function before_install_linux
{
    printf "==> before_install_linux\n"
    sudo add-apt-repository -r ppa:tmate.io/archive -y
    sudo apt-get update -q
    sudo apt-get install -y figlet \
                            coreutils \
                            libhdf5-dev \
                            cppcheck \
                            valgrind \
                            gcovr \
                            doxygen \
                            tmate 
    printf "==> before_install_linux Done\n"
 }



function build_linux
{
    figho "Building....Linux.."
    ci/bin/build.sh
    figho "Done.."
    tmate_remote_debug
}

function test_linux
{
    ci/bin/build.sh
    figho "Testing....Linux.."
    ci/bin/unittests.sh
    figho "Done.."
}

function dist_linux
{
    ci/bin/build.sh
    figho "Dist....Linux.."
    ci/bin/mk_os_distribution.sh
    ci/bin/mksdk.sh
    figho "Done.."
}

function qa_linux
{
    ci/bin/build.sh
    figho "QA....Linux.."
    ci/bin/code_coverage.sh
    ci/bin/static_code_analysis.sh
    ci/bin/memory_leak_detection.sh xml
    figho "Done.."
}


#-----------------------------
# W I N D O W S - C I
#-----------------------------
function before_install_windows
{
    printf "==> before_install_windows\n"
    choco install figlet-go
    ci/bin/install.bat $HDF5_ROOT
    printf "==> before_install_windows Done\n"
}

function build_windows
{
    figho "Building....Windows.."
    export HDF5_ROOT=./Download/hdf5
    ci/bin/build.bat
    figho "Done.."
}

function test_windows
{
    ci/bin/build.bat
    figho "Testing....Windows.."
    ci/bin/unittests.bat
    figho "Done.."
}

function dist_windows
{
    ci/bin/build.bat
    figho "Dist....Windows.."
    ci/bin/mk_os_distribution.bat
    ci/bin/mksdk.sh
    figho "Done.."
}


#-----------------------------
# M A I N - E N T R Y - C I
#-----------------------------
if (( $# != 1 )) || [[ "$TRAVIS_OS_NAME" == "" ]]
then
    printf "Usage: TRAVIS_OS_NAME=osname ${0##*/} stage\n" >&2
    exit 1
fi

# Assemple function name out of
# 1st argument and TRAVIS_OS_NAME environment
calling_function=$1_${TRAVIS_OS_NAME}

# check whether function has been defined
type $calling_function &>/dev/null
if (( $? != 0 ))
then
    figho "! error !"
    printf "Function $calling_function not defined in ${0##*/}\n"
    exit 1
else
    # function exists so ..invoke
    $calling_function
fi
