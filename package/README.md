# Instructions for the building and use IUS-SDK Software

This distribution contains a cross-platform binary distribution of the Interventional Ultrasound Library SDK. 
This document contains instructions on
* How to install the sdk
* Folder structure
* Build dependencies
* How to build and run the examples and unit tests
* How to create you own program
* Where to find SDK documentation

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
│   └── Linux
└── unitTests
    ├── include
    └── src
```

### bin
The bin folder contains convenience scripts that can be used to 
build, test and gnerate a distribution.

* build.bat - Windows build script
* build.sh - Linux/MacOs build script
* cppcheck.sh - Linux/MacOs script calling static linter
* dist.bat - Windows script generating build distribution 
* dist.sh - Linux/MacOs script, generating build distribution
* gcovr.sh - Linux/MacOs script, generating code coverage info base on unit tests
* mem.sh - Linux/MacOs script performing memory check
* mksdk.sh - Linux/MacOs script, used to generate
* ut.bat - Windows script that runs all available unit tests
* ut.sh - Linux/MacOs script that runs all available unit tests
* valgrind.sh - Linux/MacOs script that checks for memory leaks


## How to build and run the examples and unit tests
## How to create you own program
## Where to find SDK documentation