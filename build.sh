#!/bin/sh

set -e
set -x

rm -f rtspatt

# Build binary
docker build -f build_image/Dockerfile-build -t "build-rtspatt" .
docker run --rm -v $PWD:/tmp/RTSPATT -w/tmp/RTSPATT build-rtspatt

# Build image
docker build -t "rtspatt" .
