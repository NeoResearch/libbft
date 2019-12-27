#!/bin/bash

#./spec/cpp/build/app_RPCtest 0 4 1 S_drop_3_1000_500_Commit1 10 &
#./spec/cpp/build/app_RPCtest 1 4 1 S_drop_3_1000_500_Commit1 11 &
#./spec/cpp/build/app_RPCtest 2 4 1 S_drop_3_1000_500_Commit1 12


./spec/cpp/build/app_RPCtest 0 4 1 S3_2000_500_Commit1 10  > /dev/null &
./spec/cpp/build/app_RPCtest 1 4 1 S3_2000_500_Commit1 11 > /dev/null &
./spec/cpp/build/app_RPCtest 2 4 1 S3_2000_500_Commit1 12
