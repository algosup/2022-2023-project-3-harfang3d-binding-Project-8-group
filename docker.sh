#!/bin/bash

RELATIVE=$(dirname "$0")
ABSOLUTE=$(readlink -f $RELATIVE)

docker build -t fabgen $ABSOLUTE
docker run -v $ABSOLUTE:/app -it fabgen