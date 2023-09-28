#!/bin/bash

log_file="cpu_usage_log.txt"
cpu_threshold=50.0  # CPU usage threshold in percent

# Function to log process information
log_process_info() {
    pid="$1"
    end_time=$(date '+%Y-%m-%d %H:%M:%S')
    cpu_utilized="$2"
    echo "$pid,$end_time,$cpu_utilized%" >> "$log_file"
}

# Function to check and kill processes using more than the CPU usage threshold
check_and_kill_high_cpu_processes() {
    while true; do
        # Get a list of processes using more than the CPU usage threshold
        high_cpu_procs=$(ps -eo pid,%cpu --sort=-%cpu | awk -v threshold="$cpu_threshold" '$2 > threshold { print $1 }')

        if [ -n "$high_cpu_procs" ]; then
            for pid in $high_cpu_procs; do
                # Get the CPU utilization of the process before killing it
                cpu_util=$(ps -p "$pid" -o %cpu --no-headers) 

                # Log the process information before killing
                echo "Process $pid is using $cpu_util% CPU. Killing the process..."
                kill -9 "$pid"

                # Log the process information
                log_process_info "$pid" "$cpu_util"
            done
        fi

        # Sleep for 5 seconds before checking again
        sleep 5
    done
}

# Check if the log file exists, and create it if not
if [ ! -e "$log_file" ]; then
    touch "$log_file"
fi

# Start monitoring processes
check_and_kill_high_cpu_processes

