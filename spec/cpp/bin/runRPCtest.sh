#!/bin/bash

# change to base directory and execute RPC test

# requires: my_index N f H T
(cd `dirname "$0"` && ../build/app_RPCtest 0 4 1 1500 3)

