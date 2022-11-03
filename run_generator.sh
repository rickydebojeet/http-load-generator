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
'

# Variables
EXECUTABLE_NAME="generator"
THINK_TIME="0.1"
TEST_DURATION="60"
CPU="2-15"
COUNTER=1
END=20

# Function for the progress bar
function ProgressBar {
# Process data
    let _progress=(${1}*100/${2}*100)/100
    let _done=(${_progress}*4)/10
    let _left=40-$_done
# Build progressbar string lengths
    _fill=$(printf "%${_done}s")
    _empty=$(printf "%${_left}s")
printf "\rProgress : [${_fill// /#}${_empty// /-}] ${_progress}%%"
}

# Compile the load generator
make clean
make

# Make directory for results and generate the dat file
mkdir -p results
touch results.dat
> results.dat
echo -n "Load Level(users)	Throughput(req\s)	Response Time(ms)
" >> results.dat

# Run the load generator for different number of users
for i in ${USER_COUNT}; do
    ProgressBar ${COUNTER} ${END}
    taskset -a -c ${CPU} ./${EXECUTABLE_NAME} ${i} ${THINK_TIME} ${TEST_DURATION} >> runlog.log
	echo -n "${i}	" | tr '\n' ' ' >> results.dat 
	cat load_gen.log | grep "Average Throughput:" | cut -d ' ' -f 3 | tr '\n' ' ' >> results.dat
	cat load_gen.log | grep "Average Response Time:" | cut -d ' ' -f 4 >> results.dat
    mv load_gen.log results/${i}_load_gen.log
    COUNTER=$((COUNTER+1))
    echo "Done with ${i} users
    
    " >> runlog.log
done

python3 plot.py results.dat results.png "Load Testing" "Throughput (req\s)" "Response time (ms)"
mv results.dat results/results.dat
mv results.png results/results.png

echo Done!!