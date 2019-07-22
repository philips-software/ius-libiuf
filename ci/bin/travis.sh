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
    echo -e "\n\n\n" | ssh-keygen -t rsa
}


function build_linux
{
    figho "Building Linux"
    tmate
    ci/bin/build.sh
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
