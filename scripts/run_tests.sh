#!/bin/bash

set -e
set -x

# Build shamean.o
g++ -fprofile-arcs -ftest-coverage -fPIC -O0 -g -I./ -c shamean.cpp -o shamean.o  -static

# Build test file
g++ -fprofile-arcs -ftest-coverage -fPIC -O0 -g -I./ -o test_shamean test_shamean.cpp shamean.o -lcppunit -lcrypto

# Run test executable
./test_shamean

# Obtain coverage
gcovr --xml -r . > coverage.xml

