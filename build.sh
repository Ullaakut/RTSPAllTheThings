#!/bin/sh

set -e
set -x

rm -f camera_emulation_server

# Build binary
docker build -f build_image/Dockerfile-build -t "build-ces" .
docker run --rm -v $PWD:/tmp/CES -w/tmp/CES build-ces

# Build container
docker build -t "camera_emulation_server" .
