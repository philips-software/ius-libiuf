#!/bin/bash

printf "==> Running $1 on ${TRAVIS_OS_NAME}\n"

function before_install_linux
{
    printf "==> +  before_install_linux $1 on ${TRAVIS_OS_NAME}\n"
    sudo apt-get update
    sudo apt-get install -y figlet
    printf "==> -  before_install_linux Done\n"
}

# Install some custom requirements on macOS
# e.g. brew install pyenv-virtualenv
case "${TRAVIS_OS_NAME}" in
    osx)
    linux)
        eval $1_${TRAVIS_OS_NAME}
        ;;
    windows)
        # Install some custom Python 3.3 requirements on macOS
        ;;
esac
