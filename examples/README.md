# IUF file IO library SDK - examples

This folder contains two examples that demonstrate the use
of the IUF SDK.

<!-- vscode-markdown-toc -->
* [genV3file](#genV3file)
* [iufInputFileConvert](#iufInputFileConvert)

<!-- vscode-markdown-toc-config
	numbering=false
	autoSave=true
	/vscode-markdown-toc-config -->
<!-- /vscode-markdown-toc -->

## <a name='genV3file'></a>genV3file

This folder contains example code of how to construct a version 3 input file type. If you have followed the [install and build instructions](../README.md#3-a-nameBuildandtestthesoftwareaBuild-and-test-the-software) 
, an executable for your platform has been build under:

*REPO_ROOT*/build/*PLATFORM*/examples/genV3file/genV3file

Running it will result in a [HDF5](https://www.hdfgroup.org/solutions/hdf5/) file containing generated meta-data and data frames.

```
# Example for MacOS platform
$ ./build/Darwin/examples/genV3file/genV3file example.h5
Creating file example.h5, for version 93445575
$ ls -l example.h5
-rw-r--r--  1 dr  snuggles  882120 Jul 17 09:02 example.h5
$ h5dump | more
HDF5 "example.h5" {
GROUP "/" {
   GROUP "Acquisition" {
      DATASET "Date" {
         DATATYPE  H5T_STD_I32LE
         DATASPACE  SIMPLE { ( 1 ) / ( 1 ) }
         DATA {
         (0): 20180416
         }
      }
      DATASET "Description" {
         DATATYPE  H5T_STRING {
            STRSIZE 54;
            STRPAD H5T_STR_NULLTERM;
            CSET H5T_CSET_ASCII;
            CTYPE H5T_C_S1;
         }
         DATASPACE  SCALAR
         DATA {
         (0): "a nice acquisition that almost won me the nobel prize"
         }
      }
:
```

## <a name='iufInputFileConvert'></a>iufInputFileConvert

This folder contains example code of a file format converter that converts a version 2 input file type to version 3. If you have followed the [install and build instructions](../README.md#3-a-nameBuildandtestthesoftwareaBuild-and-test-the-software) 
, an executable for your platform has been build under:

*REPO_ROOT*/build/*PLATFORM*/examples/iufInputFileConvert/iufv2tov3

Running it will result in a [HDF5](https://www.hdfgroup.org/solutions/hdf5/) file containing the converted V2 file.

```
# Example for MacOS platform
$ ./build/Darwin/examples/iufInputFileConvert/iufv2tov3 example_v2.h5 example_v3.h5
successfully converted file.
Copying data
$ ls -l example_v3.h5
-rw-r--r--  1 dr  snuggles  882120 Jul 17 09:02 example.h5
$ h5dump | more
HDF5 "example.h5" {
GROUP "/" {
   GROUP "Acquisition" {
      DATASET "Date" {
         DATATYPE  H5T_STD_I32LE
         DATASPACE  SIMPLE { ( 1 ) / ( 1 ) }
         DATA {
         (0): 20180416
         }
      }
      DATASET "Description" {
         DATATYPE  H5T_STRING {
            STRSIZE 54;
            STRPAD H5T_STR_NULLTERM;
            CSET H5T_CSET_ASCII;
            CTYPE H5T_C_S1;
         }
         DATASPACE  SCALAR
         DATA {
         (0): "a nice acquisition that almost won me the nobel prize"
         }
      }
:
```