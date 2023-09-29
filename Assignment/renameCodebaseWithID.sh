#!/bin/bash

# Check for input argument
if [ "$#" -ne 1 ]; then
    echo "Usage: $0 input.csv"
    exit 1
fi

# For each line of the CSV
while IFS=, read -r col1 col2 rest; do
    # Find files with names containing the value of col1
    filename=$(find codebase/ -type f -name "*$col1*")
    if [ -z "$filename" ]; then
        echo "No submission found for <$col1>,<$col2>"
    else
        newFilename="${filename//$col1/$col2}"
        mv -v $filename $newFilename
    fi
done < "$1"
