#!/bin/sh

# set up environment variables for server IP & port
export RPC_HOST=$1

cd build
app/server > /dev/null & 2>&1
ctest -VV
pkill -SIGINT '^server$'