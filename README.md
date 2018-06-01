### Build instructions for IUS C-V3

#### Introduction

The ius C_v3 library consists of 3 folders:
- library, contains the code for the library 
- unitTests, contains unit-tests for the library
- main, contains an example of a program using the library


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
