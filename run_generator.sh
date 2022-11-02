#!/bin/bash
# Different Sizes of Users
USER_COUNT='
1
10
20
30
40
50
60
70
80
90
100
200
300
400
500
600
700
800
900
1000
'

EXECUTABLE_NAME="generator"
THINK_TIME="1"
TEST_DURATION="60"
LOG_FILE="load_gen.log"
CPU="2-15"

function ProgressBar {
# Process data
    let _progress=(${1}*100/${2}*100)/100
    let _done=(${_progress}*4)/10
    let _left=40-$_done
# Build progressbar string lengths
    _fill=$(printf "%${_done}s")
    _empty=$(printf "%${_left}s")

# 1.2 Build progressbar strings and print the ProgressBar line
# 1.2.1 Output example:                           
# 1.2.1.1 Progress : [########################################] 100%
printf "\rProgress : [${_fill// /#}${_empty// /-}] ${_progress}%%"
}
_end=100


# Compile the load generator
make clean
make

# Make directory for results
mkdir -p results

# Run the load generator for different number of users
for i in ${USER_COUNT}; do
    ProgressBar ${i} ${_end}
    taskset -c ${CPU} ./${EXECUTABLE_NAME} ${i} ${THINK_TIME} ${TEST_DURATION} >> runlog.txt
    mv ${LOG_FILE} results/${i}_${LOG_FILE}
    echo "Done with ${i} users
    
    " >> runlog.txt
done