#!/bin/bash

printf "printf stderr Running $1 on ${TRAVIS_OS_NAME}" >&2
printf "prinft stdout Running $1 on ${TRAVIS_OS_NAME}"
echo "echo stderr Running $1 on ${TRAVIS_OS_NAME}" >&2
echo "echo stdout Running $1 on ${TRAVIS_OS_NAME}"

# Install some custom requirements on macOS
# e.g. brew install pyenv-virtualenv
case "${TRAVIS_OS_NAME}" in
    osx)
        # Install some custom Python 3.2 requirements on macOS
        ;;
    linux)
        # Install some custom Python 3.3 requirements on macOS
        ;;
    windows)
        # Install some custom Python 3.3 requirements on macOS
        ;;
esac