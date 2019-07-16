# IUF file IO library SDK
[![Build Status](https://travis-ci.com/philips-software/ius-libiuf.svg?branch=develop)](https://travis-ci.com/philips-software/ius-libiuf) [![License: MIT](https://img.shields.io/badge/License-MIT-brightgreen.svg)](https://opensource.org/licenses/MIT)

**Description**:

The IUF library provides an API for reading and writing Ultrasound data and meta data to HDF5 files. Conceptually, Ultrasound data is processed in an algorithm chain starting with beamforming, followed by property extraction such as signal intensity or velocity maps and imaging steps such as compression, clutter reduction, etc. Also analysis steps such as tissue characterization can be part of the algorithm chain. Besides Ultrasound data, additional data streams, such as ECG, are relevant enhancement data.

This library facilitates such processing chains by providing abstract datatypes and functions for file IO. The HDF5 file format is used to store the following meta-data:

- acquisition properties and settings 
- processing parameters and settings 
- the history of all previous processing parameters and settings.

The added value of this library is that the data and meta-data is stored in a standardized format. This makes it easy to exchange files created with this fileio library.

**Technology stack**: The software consists of a C-library that can be used to develop an application in need of standardized Ultrasound based file io. CMake is used to build the library for the platforms supported.

**Status**:  Latest version is 2.0. Currently in devlopment is support for Python and MatLab bindings. For changes see the [CHANGELOG](CHANGELOG.md).

**Contents**
<!-- vscode-markdown-toc -->
- [IUF file IO library SDK](#IUF-file-IO-library-SDK)
  - [1. <a name='Dependencies'></a>Dependencies](#1-a-nameDependenciesaDependencies)
  - [2. <a name='Usage'></a>Usage](#2-a-nameUsageaUsage)
  - [3. <a name='Buildandtestthesoftware'></a>Build and test the software](#3-a-nameBuildandtestthesoftwareaBuild-and-test-the-software)
    - [3.1. <a name='BuildandtestinstructionsforMacLinux'></a>Build and test instructions for Mac/Linux](#31-a-nameBuildandtestinstructionsforMacLinuxaBuild-and-test-instructions-for-MacLinux)
    - [3.2. <a name='BuildandtestinstructionsforWindows'></a>Build and test instructions for Windows](#32-a-nameBuildandtestinstructionsforWindowsaBuild-and-test-instructions-for-Windows)
  - [4. <a name='ContactGettinghelp'></a>Contact / Getting help](#4-a-nameContactGettinghelpaContact--Getting-help)
  - [5. <a name='License'></a>License](#5-a-nameLicenseaLicense)
  - [6. <a name='Contributing'></a> Contributing](#6-a-nameContributinga-Contributing)
  - [7. <a name='Creditsandreferences'></a>Credits and references](#7-a-nameCreditsandreferencesaCredits-and-references)

<!-- vscode-markdown-toc-config
	numbering=true
	autoSave=true
	/vscode-markdown-toc-config -->
<!-- /vscode-markdown-toc -->

##  1. <a name='Dependencies'></a>Dependencies

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

- uuid library 
  - tested with ossp-uuid 1.6.2_2 on MacOs (Mojave 10.14.5)


##  2. <a name='Usage'></a>Usage

Examples of how the SDK can be used ara available in the [examples](examples) folder. Furthermore, developer documentation can be generated (stored in dox/html/index.html) as follows:
```
cd dox
doxygen
```

##  3. <a name='Buildandtestthesoftware'></a>Build and test the software

###  3.1. <a name='BuildandtestinstructionsforMacLinux'></a>Build and test instructions for Mac/Linux

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


###  3.2. <a name='BuildandtestinstructionsforWindows'></a>Build and test instructions for Windows

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

##  4. <a name='ContactGettinghelp'></a>Contact / Getting help

Need help or want to get in touch with us? 
* Sent an e-mail to al_libiuf@natlab.research.philips.com.
* Talk to the libiuf maintainers in the ius-libiuf-channel on Slack:
 
[![Slack](https://philips-software-slackin.now.sh/badge.svg)](https://philips-software-slackin.now.sh)

##  5. <a name='License'></a>License

This project is licensed under the [MIT](https://opensource.org/licenses/MIT) License - see the [LICENSE](LICENSE.md) file for details.

##  6. <a name='Contributing'></a> Contributing

Please refer to our [Contributing](CONTRIBUTING.md) guide when you want to contribute to this project.

##  7. <a name='Creditsandreferences'></a>Credits and references

1. THE [HDF5® LIBRARY & FILE FORMAT](https://www.hdfgroup.org/solutions/hdf5/)
2. [CMake](https://cmake.org/)
   