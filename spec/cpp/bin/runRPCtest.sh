#!/bin/bash

# this script automatically changes to base directory and execute RPC test
# requires: my_index N f H T DelayMS

# test 1: just primary alone
#(cd `dirname "$0"` && ../build/app_RPCtest 0 4 1 160 3 2000)  # primary (2 secs)

# test 2: primary on background and backup on front (2/4 nodes)
#(cd `dirname "$0"` && ../build/app_RPCtest 0 4 1 160 3 2000 > /dev/null &)  # primary (2 secs)
#(cd `dirname "$0"` && ../build/app_RPCtest 1 4 1 160 3 1000)  # backup  (1 secs)

# test 3: primary/backup on background and backup on front (3/4 nodes)
#(cd `dirname "$0"` && ../build/app_RPCtest 0 4 1 160 3 2000 > /dev/null &)  # primary (2 secs)
#(cd `dirname "$0"` && ../build/app_RPCtest 1 4 1 160 3 1000 > /dev/null &)  # backup  (1 secs)
#(cd `dirname "$0"` && ../build/app_RPCtest 2 4 1 160 3 1000)  # backup  (1 secs)

# test 4: no primary (dead). 2 backups on background and backup on front (4/4 nodes)
#(cd `dirname "$0"` && ../build/app_RPCtest 0 4 1 160 3 2000 > /dev/null &)  # primary (2 secs)
(cd `dirname "$0"` && ../build/app_RPCtest 1 4 1 160 3 1000 > /dev/null &)  # backup  (1 secs)
(cd `dirname "$0"` && ../build/app_RPCtest 2 4 1 160 3 1000 > /dev/null &)  # backup  (1 secs)
(cd `dirname "$0"` && ../build/app_RPCtest 3 4 1 160 3 1000)  # backup  (1 secs)





