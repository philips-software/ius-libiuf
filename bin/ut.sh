#!/usr/bin/env bash
BuildFolder=C_v3/build/$(uname)
echo === UnitTests
cd $BuildFolder
ctest3 --verbose

