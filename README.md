# Instructions for the building and use IUF-SDK Software
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
    $ git clone https://bitbucket.atlas.philips.com/scm/iuf/libiuf.git
    $ cd libiuf
```

## Folder structure
```
.
├── Docker
├── ci
│   └── bin
├── dox
├── examples
│   ├── genV3file
│   └── iufInputFileConvert
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
The resulting docker image is been deployed to [our local GitLab instance](#https://gitlab.ta.philips.com/IUF/iuf/container_registry).

### ci
The ci/bin folder contains convenience scripts that can be used to 
build, test and gnerate a distribution.

* build.bat - Windows build script
* build.sh - Linux/MacOs build script
* code_coverage.sh - Linux/MacOs script, generating code coverage info base on unit tests
* is_hdf5_x64 - Windows script to verify whether the correct hdf5 is installed
* make_pages.sh - Generates GitLab pages documentation 
* mk_os_distribution.bat - Windows script generating build distribution 
* mk_os_distribution.sh - Linux/MacOs script, generating build distribution
* memory_leak_detection.sh - Linux/MacOs script performing runtime memory leak detection
* mksdk.sh - Linux/MacOs script, used to collect the artefacts from the mk_os_distribution scripts and generate an SDK folder
* static_code_analysis.sh - Linux/MacOs script running static code analysis
* unittests.bat - Windows script that runs all available unit tests
* unittests.sh - Linux/MacOs script that runs all available unit tests

### dox
The dox folder contains the Doxyfile that can be used to generate the API documentation.

### examples
The examples folder contains examples that demonstrate the use of the IUF fileio
library.
* iufInputFileConvert - converts the V2 input file to V3 input file
* genV3file - just a iuf file generator, with some arbitrary data

### external
This folder needs the external dependencies, needed to build the software.

### include
This folder contains the IUF-SDK header files.

### library
This folder contains the IUF-SDK platform specific static library files, both
compiled for debug and release mode.

### python
This folder contains the code needed to build the Python 2 and 3 bindings for
teh IUS-SDk.

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
  - tested with AppleClang 10.0.0.10001044 on MacOS (Mojave 10.14.3)
  
- cmake
  - tested with 3.14.0 on Windows (7/10 pro)
  - tested with 3.14.0 on Linux (centos:7.4.1708)
  - tested with 3.14.0 on MacOs (Sierra 10.12.6)
  
- hdf5 library
  - tested with 1.8.20 on Windows (7/10 pro)
  - tested with 1.8.12 on Linux (centos:7.4.1708)
  - tested with 1.8.20 on MacOs (Sierra 10.12.6)

- numpy
  - tested with 1.16.2 for Python 2.7.5 on Linux (centos:7.4.1708)
  - tested with 1.16.2 for Python 3.6.6 on Linux (centos:7.4.1708)
  - tested with 1.8.0rc1 for Python 2.7.10 on MacOS (Mojave 10.14.3)
  - tested with 1.16.2 for Python 3.7.2 on MacOS (Mojave 10.14.3)
   
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
Start the Visual Studio x64 command prompt and run ci\bin\is_hdf5_x64.bat. This should show output similar
to the text shown below: 
```
c:\proj\libiuf> ci\bin\is_hdf5_x64.bat
HDF5 lib architecture [C:\Program Files\HDF_Group\HDF5\1.8.20]:
            8664 machine (x64)
```

If this is not the case, point the PATH environment to the x64 version of hdf5 bin folder and try again.


- Checkout and build code

    ```
    c:\proj\libiuf> ci\bin\build.bat
    ```
- Run unit tests

    ```
    c:\proj\libiuf> ci\bin\unittests.bat
    Unity test run 1 of 1
    .......................
    
    -----------------------
    23 Tests 0 Failures 0 Ignored 
    OK
    
    Process finished with exit code 0
    ```
- Build a dist folder containing the distributable SDK:

    ```
    c:\proj\libiuf> ci\bin\mk_os_distribution.bat
    ```

### How to generate documentation
```
cd dox
doxygen
```
