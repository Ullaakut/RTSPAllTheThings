#!/bin/sh

set -e
set -x

rm -f ces

# Build binary
docker build -f build_image/Dockerfile-build -t "build-ces" .
docker run --rm -v $PWD:/tmp/CES -w/tmp/CES build-ces

# Build image
docker build -t "ces" .
