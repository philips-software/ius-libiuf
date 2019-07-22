#!/bin/bash
set -e
function figho
{
    echo $* | figlet
}

function tmate_remote_debug
{
    figho "Tmate session.."
    echo  | ssh-keygen -t rsa -N ""
    tmate -S /tmp/tmate.sock new-session -d               # Launch tmate in a detached state
    tmate -S /tmp/tmate.sock wait tmate-ready             # Blocks until the SSH connection is established
    tmate -S /tmp/tmate.sock display -p '#{tmate_ssh}'    # Prints the SSH connection string
    while true
    do
        echo $SECONDS
        TMATEPID=$(ps -eaf | awk '/.*tmate \-S.* -d/ { print $2 }')
        if [[ $TMATEPID == "" ]]
        then
            break
        fi
        sleep 10
    done
    figho "Tmate closed.."
}

function before_install_linux
{
    printf "==> before_install_linux\n"
    sudo add-apt-repository -r ppa:tmate.io/archive -y
    sudo apt-get update -q
    sudo apt-get install -y figlet \
                            coreutils \
                            libhdf5-dev \
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


# Install some custom requirements on macOS
# e.g. brew install pyenv-virtualenv
case "${TRAVIS_OS_NAME}" in
    osx|linux)
        export HDF5_ROOT=
        eval $1_${TRAVIS_OS_NAME}
        ;;
    windows)
        eval $1_${TRAVIS_OS_NAME}
        ;;
esac
