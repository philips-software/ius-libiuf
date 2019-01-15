# Instructions for the building and use IUS-SDK Software
This repository contains the source code of the Interventional Ultrasound Library SDK. 
This document contains instructions on
* [How to install the sdk sources](#how-to-install-the-sdk)
* [Folder structure](#folder-structure)
* [Build dependencies](#build-dependencies)
* [How to build and run the examples and unit tests](#how-to-build-and-run-the-examples-and-unit-tests)
* [How to generate documentation](#how-to-generate-documentation)


## How to install the sdk sources
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
```

### Checkout code
```
    $ git clone https://bitbucket.atlas.philips.com/scm/ius/libius.git
    $ cd libius
    $ git submodule update --init --recursive
    $ git checkout develop
```

## Folder structure
```
.
├── Docker
├── ci
│   └── bin
├── dox
│   └── img
├── examples
│   ├── genV3file
│   └── iusInputFileConvert
├── external
│   ├── Unity
│   ├── cmake_modules
│   ├── hashmap
│   └── hdf5_local
├── library
│   ├── include
│   └── src
├── package
└── unitTests
    ├── include
    └── src

```
### Docker
This folder contains the Dockerfile that is is used to build/develop for the Linux platform.
The resulting docker image is been deployed to [our local GitLab instance](#https://gitlab.ta.philips.com/IUS/ius/container_registry).

### ci
The ci/bin folder contains convenience scripts that can be used to 
build, test and gnerate a distribution.

* build.bat - Windows build script
* build.sh - Linux/MacOs build script
* static_code_analysis.sh - Linux/MacOs script running static code analysis
* mk_os_distribution.bat - Windows script generating build distribution 
* mk_os_distribution.sh - Linux/MacOs script, generating build distribution
* code_coverage.sh - Linux/MacOs script, generating code coverage info base on unit tests
* mksdk.sh - Linux/MacOs script, used to collect the artefacts from the mk_os_distribution scripts and generate an SDK folder
* unittests.bat - Windows script that runs all available unit tests
* unittests.sh - Linux/MacOs script that runs all available unit tests
* memory_leak_detection.sh - Linux/MacOs script performing runtime memory leak detection

### dox
The dox folder contains the Doxyfile that can be used to generate the API documentation.

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

### package
This folder contains files that will be added to the SDK distribution (dist.sh/dist.bat).

### unitTests
This folder contains the unitTest the library has been tested with. This folder also 
serves as an inspiration source of the API usage.

## Build requirements
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

#### Build instructions for Mac/Linux

- Build code

    ```
    $ ci/bin/build.sh
    ```
- Run unit tests

    ```
    $ ci/bin/unittests.sh
    Unity test run 1 of 1
    .......................
    
    -----------------------
    23 Tests 0 Failures 0 Ignored 
    OK
    
    Process finished with exit code 0
    ```
- Generate SDK:
    ```
    $ uname
    Linux
    $ ci/bin/mk_os_distribution.sh
    $ cd build/Linux
    $ ls -l dist
    
      total 8
      -rw-r--r--  1 dr snuggles 1133 Oct 11 08:49 CMakeLists.txt
      drwxr-xr-x  3 dr snuggles  102 Oct 11 10:41 bin
      drwxr-xr-x  4 dr snuggles  136 Oct 11 10:41 examples
      drwxr-xr-x  8 dr snuggles  272 Oct 11 10:41 external
      drwxr-xr-x 53 dr snuggles 1802 Oct 11 10:41 include
      drwxr-xr-x  3 dr snuggles  102 Oct 11 10:41 lib
      -rw-r--r--  1 dr snuggles  296 Oct 11 08:49 PreLoad.cmake
      drwxr-xr-x  5 dr snuggles  170 Oct 11 10:41 unitTests

    ```


#### Build instructions for Windows

- When building for Windows, make sure the Visual Studio runtime version of the hdf5 library matches
with the installed version of Visual Studio. Visual Studio 2017 for example, 
[requires version 1.8.20 of the hdf5 library](https://portal.hdfgroup.org/display/support/HDF5+1.8.20#files). 


- Also, make sure the x64 version of the hdf5 library is used. (The downloaded filename contains _64 in its name.
For example [hdf5-1.8.20-Std-win7_64-vs14.zip](https://support.hdfgroup.org/ftp/HDF5/releases/hdf5-1.8/hdf5-1.8.20/bin/windows/hdf5-1.8.20-Std-win7_64-vs14.zip)).
Start the Visual Studio x64 command prompt and run h5cc --version. This should show x64 in the Target property.
If this is not the case, point the PATH environment to the x64 version of hdf5 bin folder and try again.

- Checkout and build code

    ```
    c:\proj\libius> ci\bin\build.bat
    ```
- Run unit tests

    ```
    c:\proj\libius> ci\bin\unittests.bat
    Unity test run 1 of 1
    .......................
    
    -----------------------
    23 Tests 0 Failures 0 Ignored 
    OK
    
    Process finished with exit code 0
    ```
- Build a dist folder containing the distributable SDK:

    ```
    c:\proj\libius> ci\bin\mk_os_distribution.bat
    ```

### How to generate documentation
```
cd dox
doxygen
```
