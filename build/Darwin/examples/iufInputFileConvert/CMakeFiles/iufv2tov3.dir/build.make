# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.13.2/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.13.2/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/frankvanheesch/Documents/GitHub/ius-libiuf

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin

# Include any dependencies generated for this target.
include examples/iufInputFileConvert/CMakeFiles/iufv2tov3.dir/depend.make

# Include the progress variables for this target.
include examples/iufInputFileConvert/CMakeFiles/iufv2tov3.dir/progress.make

# Include the compile flags for this target's objects.
include examples/iufInputFileConvert/CMakeFiles/iufv2tov3.dir/flags.make

examples/iufInputFileConvert/CMakeFiles/iufv2tov3.dir/src/main.c.o: examples/iufInputFileConvert/CMakeFiles/iufv2tov3.dir/flags.make
examples/iufInputFileConvert/CMakeFiles/iufv2tov3.dir/src/main.c.o: ../../examples/iufInputFileConvert/src/main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object examples/iufInputFileConvert/CMakeFiles/iufv2tov3.dir/src/main.c.o"
	cd /Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin/examples/iufInputFileConvert && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/iufv2tov3.dir/src/main.c.o   -c /Users/frankvanheesch/Documents/GitHub/ius-libiuf/examples/iufInputFileConvert/src/main.c

examples/iufInputFileConvert/CMakeFiles/iufv2tov3.dir/src/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/iufv2tov3.dir/src/main.c.i"
	cd /Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin/examples/iufInputFileConvert && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/frankvanheesch/Documents/GitHub/ius-libiuf/examples/iufInputFileConvert/src/main.c > CMakeFiles/iufv2tov3.dir/src/main.c.i

examples/iufInputFileConvert/CMakeFiles/iufv2tov3.dir/src/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/iufv2tov3.dir/src/main.c.s"
	cd /Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin/examples/iufInputFileConvert && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/frankvanheesch/Documents/GitHub/ius-libiuf/examples/iufInputFileConvert/src/main.c -o CMakeFiles/iufv2tov3.dir/src/main.c.s

examples/iufInputFileConvert/CMakeFiles/iufv2tov3.dir/src/readInputFileV2.c.o: examples/iufInputFileConvert/CMakeFiles/iufv2tov3.dir/flags.make
examples/iufInputFileConvert/CMakeFiles/iufv2tov3.dir/src/readInputFileV2.c.o: ../../examples/iufInputFileConvert/src/readInputFileV2.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object examples/iufInputFileConvert/CMakeFiles/iufv2tov3.dir/src/readInputFileV2.c.o"
	cd /Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin/examples/iufInputFileConvert && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/iufv2tov3.dir/src/readInputFileV2.c.o   -c /Users/frankvanheesch/Documents/GitHub/ius-libiuf/examples/iufInputFileConvert/src/readInputFileV2.c

examples/iufInputFileConvert/CMakeFiles/iufv2tov3.dir/src/readInputFileV2.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/iufv2tov3.dir/src/readInputFileV2.c.i"
	cd /Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin/examples/iufInputFileConvert && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/frankvanheesch/Documents/GitHub/ius-libiuf/examples/iufInputFileConvert/src/readInputFileV2.c > CMakeFiles/iufv2tov3.dir/src/readInputFileV2.c.i

examples/iufInputFileConvert/CMakeFiles/iufv2tov3.dir/src/readInputFileV2.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/iufv2tov3.dir/src/readInputFileV2.c.s"
	cd /Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin/examples/iufInputFileConvert && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/frankvanheesch/Documents/GitHub/ius-libiuf/examples/iufInputFileConvert/src/readInputFileV2.c -o CMakeFiles/iufv2tov3.dir/src/readInputFileV2.c.s

# Object files for target iufv2tov3
iufv2tov3_OBJECTS = \
"CMakeFiles/iufv2tov3.dir/src/main.c.o" \
"CMakeFiles/iufv2tov3.dir/src/readInputFileV2.c.o"

# External object files for target iufv2tov3
iufv2tov3_EXTERNAL_OBJECTS =

examples/iufInputFileConvert/iufv2tov3: examples/iufInputFileConvert/CMakeFiles/iufv2tov3.dir/src/main.c.o
examples/iufInputFileConvert/iufv2tov3: examples/iufInputFileConvert/CMakeFiles/iufv2tov3.dir/src/readInputFileV2.c.o
examples/iufInputFileConvert/iufv2tov3: examples/iufInputFileConvert/CMakeFiles/iufv2tov3.dir/build.make
examples/iufInputFileConvert/iufv2tov3: library/libiuf-3.37750b8d.a
examples/iufInputFileConvert/iufv2tov3: /usr/local/Cellar/hdf5/1.10.4/lib/libhdf5.dylib
examples/iufInputFileConvert/iufv2tov3: /usr/local/opt/szip/lib/libsz.dylib
examples/iufInputFileConvert/iufv2tov3: /usr/lib/libz.dylib
examples/iufInputFileConvert/iufv2tov3: /usr/lib/libdl.dylib
examples/iufInputFileConvert/iufv2tov3: /usr/lib/libm.dylib
examples/iufInputFileConvert/iufv2tov3: /usr/local/Cellar/hdf5/1.10.4/lib/libhdf5_hl.dylib
examples/iufInputFileConvert/iufv2tov3: examples/iufInputFileConvert/CMakeFiles/iufv2tov3.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable iufv2tov3"
	cd /Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin/examples/iufInputFileConvert && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/iufv2tov3.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
examples/iufInputFileConvert/CMakeFiles/iufv2tov3.dir/build: examples/iufInputFileConvert/iufv2tov3

.PHONY : examples/iufInputFileConvert/CMakeFiles/iufv2tov3.dir/build

examples/iufInputFileConvert/CMakeFiles/iufv2tov3.dir/clean:
	cd /Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin/examples/iufInputFileConvert && $(CMAKE_COMMAND) -P CMakeFiles/iufv2tov3.dir/cmake_clean.cmake
.PHONY : examples/iufInputFileConvert/CMakeFiles/iufv2tov3.dir/clean

examples/iufInputFileConvert/CMakeFiles/iufv2tov3.dir/depend:
	cd /Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/frankvanheesch/Documents/GitHub/ius-libiuf /Users/frankvanheesch/Documents/GitHub/ius-libiuf/examples/iufInputFileConvert /Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin /Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin/examples/iufInputFileConvert /Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin/examples/iufInputFileConvert/CMakeFiles/iufv2tov3.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : examples/iufInputFileConvert/CMakeFiles/iufv2tov3.dir/depend

