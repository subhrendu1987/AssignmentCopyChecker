#!/bin/bash

input_file="input_numbers.txt"
output_file="output_numbers.txt"

if [ ! -f "$input_file" ]; then
    echo "Input file not found: $input_file"
    exit 1
fi

while IFS=$'\t' read -r number base_input base_conversion; do
    result=$(echo "ibase=$base_input;obase=$base_conversion;$number" | bc)
    echo "$result" >> "$output_file"
done < "$input_file"

echo "Conversion completed. Results are stored in $output_file."

