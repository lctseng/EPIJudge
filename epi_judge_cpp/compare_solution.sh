#!/bin/bash
SRC_DIR=.
LAST_MODIFIED_CXX_FILE=`ls -rt $SRC_DIR/*\.cc | tail -1`
TARGET=$1
if [ -z $1]; then
  TARGET=`basename $LAST_MODIFIED_CXX_FILE | awk -F. '{print \$1}'`
fi
echo ">>> Running test for $TARGET.cc <<<"
echo "======== Your answer ========"
make $TARGET
echo
echo "===== Provided solution ====="
cd ../epi_judge_cpp_solutions && make $TARGET && cd - 
echo ">>> Finished test for $TARGET.cc <<<"