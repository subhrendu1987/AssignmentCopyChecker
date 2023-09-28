#!/bin/bash

# Input file containing numbers and base information
input_file="input_numbers.txt"

# Output file to store the converted numbers
output_file="output_numbers.txt"

# Function to validate and convert a number from one base to another
convert_base() {
    number="$1"
    from_base="$2"
    to_base="$3"
    
    # Check if the input number is valid in the specified base
    if ! [[ $number =~ ^[0-9A-Fa-f]+$ ]]; then
        echo "Invalid input number: $number"
        return
    fi
    
    converted_number=$(echo "ibase=$from_base;obase=$to_base;$number" | bc 2>/dev/null)
    
    if [ -n "$converted_number" ]; then
        echo -e "$number\t$from_base\t$to_base\t$converted_number" >> "$output_file"
    else
        echo "Conversion failed for $number (from base $from_base to base $to_base)"
    fi
}

# Process the input file and write the results to the output file
while IFS=$'\t' read -r number from_base to_base; do
    convert_base "$number" "$from_base" "$to_base"
done < "$input_file"

echo "Conversion completed. Results are stored in $output_file."
