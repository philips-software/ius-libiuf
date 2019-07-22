#!/bin/bash

function figho
{
    echo $* | figlet
}

function before_install_linux
{
    printf "==> before_install_linux\n"
    sudo apt-get install -y software-properties-common
    sudo add-apt-repository ppa:tmate.io/archive -y
    sudo apt-get update
    sudo apt-get install -y figlet \
                            coreutils \
                            software-properties-common \
                            tmate 
    printf "==> before_install_linux Done\n"
    echo  | ssh-keygen -t rsa -N ""
 }


function tmate_remote_debug
{
    figho "Tmate session.."  
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

function build_linux
{
    figho "Building....Linux.."
    ci/bin/build.sh
    tmate_remote_debug
}

# Install some custom requirements on macOS
# e.g. brew install pyenv-virtualenv
case "${TRAVIS_OS_NAME}" in
    osx|linux)
        eval $1_${TRAVIS_OS_NAME}
        ;;
    windows)
        # Install some custom Python 3.3 requirements on macOS
        ;;
esac
