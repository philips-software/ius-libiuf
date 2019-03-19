# Instructions for building and using the mex wrapper for the IUF-SDK Software
This wrapper provides a MatLab mex interface to the IUF-SDK Software functions.

This document contains instructions on
* [How to install MatLab](#how-to-install-matlab)
* [How to build the wrapper using MatLab]
* [How to build the wrapper using command prompt]

## How to install MatLab
### Philips Research specific installation instructions for Linux (cluster running centos 7.4)

    # login on compute server
    $ ssh llogin
      Welcome to aharna running Linux 3.10.0-693.11.6.el7.x86_64 

    $ hosts $(hostname)
    login

    $ qsh
    Started on lato in isar

    $ hosts $(hostname)
    compute
    
    # install gcovr build dependencies using cadenv
    $ cadenv MatLab
    

### Windows
    # Use the software portal

## How to build the wrapper using MatLab

- Build the IUF-SDK library
- Open MatLab and run the function mxCreate.m in the directory ./src/

## How to build the wrapper using command prompt:

- Windows:
- Open the 'x64 Native Tools Command Prompt for VS 2017' and run build.bat in the directory ./src/

- Linux
- Run the command build.sh in the directory ./src/
