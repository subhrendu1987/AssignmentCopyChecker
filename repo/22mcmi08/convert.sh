#!/bin/bash

# Check if the correct number of arguments is provided
if [ "$#" -ne 3 ]; then
  echo "Usage: $0 Input_number Base_of_Input Base_of_conversion"
  exit 1
fi

input_number="$1"
input_base="$2"
output_base="$3"

# Check if the input base and output base are valid (between 2 and 36)
if [ "$input_base" -lt 2 ] || [ "$input_base" -gt 36 ] || [ "$output_base" -lt 2 ] || [ "$output_base" -gt 36 ]; then
  echo "Invalid base. Base must be between 2 and 36."
  exit 1
fi

# Convert the input number to the desired base using 'bc'
converted_number=$(echo "obase=$output_base; ibase=$input_base; $input_number" | bc)

# Store the conversion result in a text file
echo "Input number: $input_number (Base $input_base)" > conversion_result.txt
echo "Converted number: $converted_number (Base $output_base)" >> conversion_result.txt

echo "Conversion complete. Results saved in conversion_result.txt"

