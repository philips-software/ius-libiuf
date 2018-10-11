### Build instructions for IUS C-V3

#### Introduction

The ius C_v3 library consists of 3 folders:
- library, contains the code for the library 
- unitTests, contains unit-tests for the library
- examples, contains an example of a program using the library

#### Build requirements
- Install cmake
- Install hdf5 library

#### Build instructions for Mac/Linux

- Checkout and build code

    ```
    $ git clone https://USERID@bitbucket.atlas.philips.com/scm/ius/ius.git
    $ cd ius
    $ git checkout feature/IUS-99-input-file-io-test
    $ C_v3/bin/build.sh
    ```
- Run unit tests

    ```
    $ C_v3/bin/ut.sh
    Unity test run 1 of 1
    .......................
    
    -----------------------
    23 Tests 0 Failures 0 Ignored 
    OK
    
    Process finished with exit code 0
    ```

- The next example will build a dist folder contining 
the distributable SDK:
    ```
    $ uname
    Linux
    $ C_v3/bin/dist.sh
    $ cd C_v3/build/Linux
    $ ls -l dist
    
      total 8
      -rw-r--r--  1 dr snuggles 1133 Oct 11 08:49 CMakeLists.txt
      drwxr-xr-x  4 dr snuggles  136 Oct 11 10:41 examples
      drwxr-xr-x  8 dr snuggles  272 Oct 11 10:41 external
      drwxr-xr-x 53 dr snuggles 1802 Oct 11 10:41 include
      drwxr-xr-x  3 dr snuggles  102 Oct 11 10:41 lib
      -rw-r--r--  1 dr snuggles  296 Oct 11 08:49 PreLoad.cmake
      drwxr-xr-x  5 dr snuggles  170 Oct 11 10:41 unitTests

    ```


#### Build instructions for Windows

- Checkout and build code

    ```
    c:\proj> git clone https://USERID@bitbucket.atlas.philips.com/scm/ius/ius.git
    c:\proj> cd ius
    c:\proj> git checkout feature/IUS-99-input-file-io-test
    c:\proj> C_v3\bin\build.bat
    ```
- Run unit tests

    ```
    c:\proj> C_v3\bin\ut.bat
    ```
