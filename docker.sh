#!/bin/bash

RELATIVE=$(dirname "$0")
ABSOLUTE=$(readlink -f $RELATIVE)

docker build -t fabgen $ABSOLUTE
# docker run -v $ABSOLUTE:/app --rm -it fabgen python3 tests.py --linux --rust
docker run -v $ABSOLUTE:/app --rm -it fabgen bash