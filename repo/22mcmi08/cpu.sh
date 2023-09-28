#!/bin/bash

# Log file to store process information
LOG_FILE="cpu_utilization.log"

# Function to check CPU utilization
check_cpu_utilization() {
    local threshold=50.0
    ps -eo pid,%cpu --sort=-%cpu | awk -v threshold="$threshold" '$2 > threshold {print $1, strftime("%Y-%m-%d %H:%M:%S"), $2}' | while read -r pid end_time cpu_utilization; do
        echo "Killing PID: $pid (CPU Utilization: $cpu_utilization%)"
        echo "$pid,$end_time,$cpu_utilization%" >> "$LOG_FILE"
        kill -9 "$pid"
    done
}

# Check CPU utilization periodically (you can change the sleep interval as needed)
while true; do
    check_cpu_utilization
    sleep 5  # Check every 5 seconds
done
