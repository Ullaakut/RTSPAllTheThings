#!/bin/sh

set -e
set -x

# Build image
docker build -t "rtspatt" .
