#!/usr/bin/env bash
echo === Building ius
(cd C_v3 ; make )
echo === UnitTests
(cd C_v3/unitTests ; make valgrind)

