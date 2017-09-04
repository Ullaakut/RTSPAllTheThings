#!/bin/bash

set -e
set -x

# Build RTSPATT
mkdir -p ../build
cd ../build
cmake ../RTSPATT
make

# mv RTSPATT bin to mounted dir
mv rtspatt ../RTSPATT
