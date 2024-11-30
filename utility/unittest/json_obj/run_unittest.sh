#!/bin/bash

unittest_path=`pwd`
test_target_path=$unittest_path/../../

cd $test_target_path && echo "complite in `pwd`" && make clean && make -j 
if [[ $? -ne 0 ]]; then
    echo "complite failed"
    exit -1
fi

cd $unittest_path
echo "exec unittest in `pwd`"

export LD_LIBRARY_PATH=../../../bin

echo "$1"
if [[ "$1" == "gdb" ]]; then
    make clean && make && gdb $PWD/unittest.out
else
    make clean && make && $PWD/unittest.out
fi