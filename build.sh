#!/bin/sh

set -e
set -x

rm -f camera_emulation_server
docker build -f build_image/Dockerfile-build -t "build-ces" .
docker run --rm -v $PWD:/tmp/CES -w/tmp/CES build-ces
