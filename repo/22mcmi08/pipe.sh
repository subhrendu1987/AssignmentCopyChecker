#!/bin/bash

# Create pipes
pipe1=$(mktemp -u)
pipe2=$(mktemp -u)
mkfifo "$pipe1" "$pipe2"

# First child: ls command
(ls > "$pipe1") &

# Second child: grep command
(grep '^A.*\.txt$' < "$pipe1" > "$pipe2") &

# Third child: wc command
(wc -l < "$pipe2") &

# Wait for all children to finish
wait

# Clean up temporary pipes
rm -f "$pipe1" "$pipe2"

