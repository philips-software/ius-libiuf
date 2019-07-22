#!/bin/bash

function figho
{
    echo $* | figlet
}

function before_install_linux
{
    printf "==> before_install_linux\n"
    sudo apt-get install -y software-properties-common
    sudo add-apt-repository ppa:tmate.io/archive
    sudo apt-get update
    sudo apt-get install -y figlet \
                            coreutils \
                            software-properties-common \
                            tmate 
    printf "==> before_install_linux Done\n"
}


function build_linux
{
    figho "Building Linux"
    ci/bin/build.sh
    tmate
}

# Install some custom requirements on macOS
# e.g. brew install pyenv-virtualenv
case "${TRAVIS_OS_NAME}" in
    osx|linux)
        rm -rf build
        eval $1_${TRAVIS_OS_NAME}
        ;;
    windows)
        # Install some custom Python 3.3 requirements on macOS
        ;;
esac
