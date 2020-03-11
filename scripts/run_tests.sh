#!/bin/bash

set -e
set -x

# Build shamean.o
g++ -fprofile-arcs -ftest-coverage -fPIC -O0 -g -I./ -c shamean.cpp -o shamean.o  -static

# Build test file
g++ -fprofile-arcs -ftest-coverage -fPIC -O0 -g -I./ -o testshamean TestShamean.cpp shamean.o -lcppunit -lcrypto

# Run test executable
./testshamean

# Obtain coverage
gcovr --xml -r . > coverage.xml

