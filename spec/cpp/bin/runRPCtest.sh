#!/bin/bash

# change to base directory and execute RPC test

# requires: my_index N f H T DelayMS
(cd `dirname "$0"` && ../build/app_RPCtest 0 4 1 160 3 2000 > /dev/null &)  # primary (2 secs)
(cd `dirname "$0"` && ../build/app_RPCtest 1 4 1 160 3 1000)  # backup  (1 secs)

#(cd `dirname "$0"` && ../build/app_RPCtest 0 4 1 160 3 2000)  # primary (2 secs)


