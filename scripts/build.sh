#!/bin/bash

set -e
set -x

# Build!
g++ -g main.cpp shamean.cpp -o shamean -lcrypto -static

