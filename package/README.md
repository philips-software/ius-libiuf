# Instructions for the building and use IUS-SDK Software

This distribution contains a cross-platform binary distribution of the Interventional Ultrasound Library SDK. 
This document contains instructions on
* [How to install the sdk](#how-to-install-the-sdk)
* [Folder structure](#folder-structure)
* [Build dependencies](#build-dependencies)
* [How to build and run the examples and unit tests](#how-to-build-and-run-the-examples-and-unit-tests)
* [How to create you own program](#how-to-create-you-own-program)
* [Where to find SDK documentation](#where-to-find-sdk-documentation)

## How to install the sdk
Easy (:). The SDK comes as a zipped archive. Installation only means 
unpacking this archive with your favorite unpacking tool.

## Folder structure
When installed, the sdk contains a file structure that is similar to:

```
.
├── bin
├── documentation
│   ├── html
│   └── img
├── examples
│   ├── genV3file
│   └── iusInputFileConvert
├── external
│   ├── Unity
│   ├── cmake_modules
│   ├── hashmap
│   └── hdf5_local
├── include
├── library
│   ├── Linux
│   └── Windows
└── unitTests
    ├── include
    └── src
```

### bin
The bin folder contains convenience scripts that can be used to 
build, test and gnerate a distribution.

* build.bat - Windows build script
* build.sh - Linux/MacOs build script
* static_code_analysis.sh - Linux/MacOs script running static code analysis
* code_coverage.sh - Linux/MacOs script, generating code coverage info base on unit tests
* unittests.bat - Windows script that runs all available unit tests
* unittests.sh - Linux/MacOs script that runs all available unit tests
* memory_leak_detection.sh - Linux/MacOs script performing runtime memory leak detection

### documentation
The html folder contains the generated API documentation. Main file
is index.html.

### examples
The examples folder contains examples that demonstrate the use of the IUS fileio 
library.
* iusInputFileConvert - converts the V2 input file to V3 input file
* genV3file - just a ius file generator, with some arbitrary data

### external
This folder needs the external dependencies, needed to build the software.

### include
This folder contains the IUS-SDK header files.

### library
This folder contains the IUS-SDK platform specific static library files, both 
compiled for debug and release mode.

### unitTests
This folder contains the unitTest the library has been tested with. This folder also 
serves as an inspiration source of the API usage.

## How to build and run the examples and unit tests
### Build requirements
In order to build and test the code, the required packages need to be installed:
- compiler
  - tested with MSVC 19.0.24215.1 on Windows 7
  - tested with GNU c compiler 4.8.5 on Linux (centos:7.4.1708)
  - tested with AppleClang 9.0.0.9000039 on MacOS (Sierra 10.12.6)
  
- cmake
  - tested with 3.10.3 on Windows (7/10 pro)
  - tested with 3.12.1 on Linux (centos:7.4.1708)
  - tested with 3.10.2 on MacOs (Sierra 10.12.6)
  
- hdf5 library
  - tested with 1.8.20 on Windows (7/10 pro)
  - tested with 1.8.12 on Linux (centos:7.4.1708)
  - tested with 1.8.20 on MacOs (Sierra 10.12.6)

- gcovr
  - tested with 4.1 on Linux (centos:7.4.1708)
  - tested with 4.1 on MacOs (Sierra 10.12.6)
 
### Philips Research specific installation instructions for Linux (cluster running centos 7.4)
```
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
    $ cadenv -r 2.7-64 python
    
    # extract SDK
    $ unzip IUS-SDK-3.1.0.zip
    $ cd dist/IUS-SDK-3.1.0
```

### Build instructions for Mac/Linux
- Build code
    ```
    $ bin/build.sh
    ```
- Run unit tests

    ```
    $ bin/unittests.sh
    Unity test run 1 of 1
    .......................
    
    -----------------------
    23 Tests 0 Failures 0 Ignored 
    OK
    
    Process finished with exit code 0
    ```

### Build instructions for Windows

- Build code
    ```
    c:\> bin\build.bat
    ```
- Run unit tests

    ```
    C:\> bin\unittests.bat
    Unity test run 1 of 1
    .......................
    
    -----------------------
    23 Tests 0 Failures 0 Ignored 
    OK
    
    Process finished with exit code 0
    ```

## How to create you own program
The examples folder can be used to integrate you own program into 
build process. Steps are as follows:
- Create a new sub-folder of the examples folder, say myIUSapp
- Add subfolder myIUSapp to the build process by extendig the examples/CmakeLists.txt 
file with the statement: add_subdirectory(myIUSapp)
- Create your sources and CMakeLists.txt in the myIUSapp folder, similar to 
the other examples.

## Where to find SDK documentation
Ths SDK API Documentation can be found in documentation/html/index.html
