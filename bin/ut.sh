#!/usr/bin/env bash
echo === UnitTests
cd C_v3/unitTests 
exec make valgrind

