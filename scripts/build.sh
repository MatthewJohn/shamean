#!/bin/bash

set -e
set -x

# Build!
g++ -Wall -Wextra -g main.cpp shamean.cpp -o shamean -lcrypto -static

