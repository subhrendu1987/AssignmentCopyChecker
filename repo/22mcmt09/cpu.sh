#!/bin/bash

#LOGFILE
LOG_FILE="cpu_log.txt"

# Check if log file exists, if not, then create it with the headers
if [ ! -f "$LOG_FILE" ]; then
    echo "PID,End Time,CPU Utilized (%)" > "$LOG_FILE"
fi  
# command has succesfully executed
#LOGFILE exists

# Here Defining the CPU threshold as given in the question
CPU_THRESHOLD=50

# Checking the processes
#Here, we are using "ps" as "ps" command is use to express the percentage of time spent running during the entire lifetime of a process.
ps -eo pid,%cpu --sort=-%cpu | while read -r line; do
    if [[ $line == "PID" ]]; then
        continue
    fi
# command has succesfully executed

    PID=$(echo "$line" | awk '{print $1}')
    CPU_UTIL=$(echo "$line" | awk '{print $2}')
#Here, we used "awk" as it is useful in generating repports and it requires no compilation
#"awk" also allows users to use variables,numeric values,etc.


    # Check if CPU utilization exceeds threshold or not
    if (( $(echo "$CPU_UTIL > $CPU_THRESHOLD" | bc -l) )); then
        # Log the process details
        END_TIME=$(date +"%Y-%m-%d %H:%M:%S")
        echo "$PID,$END_TIME,$CPU_UTIL" >> "$LOG_FILE"
        
        #  Now Kill the process
        kill -9 $PID
        echo "Process $PID killed due to high CPU utilization"
    fi
# command has succesfully executed
#the process has been killed
done