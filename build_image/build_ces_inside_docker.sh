#!/bin/bash

set -e
set -x

# Build CES
mkdir ../build
cd ../build
cmake ../CES
make

# mv CES bin to mounted dir
mv ces ../CES
