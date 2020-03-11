#!/bin/bash

set -e
set -x

TAG_NAME=shamean-build

# Perform build
docker build . -t $TAG_NAME -f Dockerfile.build $@

echo Build container built and tagged: $TAG_NAME

