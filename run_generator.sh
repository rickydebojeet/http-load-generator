#!/bin/bash
# Different Sizes of Users
USER_COUNT='
100
'

EXECUTABLE_NAME="generator"
THINK_TIME="0.5"
TEST_DURATION="60"
LOG_FILE="load_gen.log"
CPU="1-4"

# Compile the load generator
make clean
make

# Make directory for results
mkdir -p results

# Run the load generator for different number of users
for i in ${USER_COUNT}; do
    taskset -c ${CPU} ./${EXECUTABLE_NAME} ${i} ${THINK_TIME} ${TEST_DURATION} >> runlog.txt
    mv ${LOG_FILE} results/${i}_${LOG_FILE}
done