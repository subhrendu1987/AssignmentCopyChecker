#!/bin/bash

# Here Define the input file and output file as mentioned in the question
INPUT_FILE="input_numbers.txt"
OUTPUT_FILE="converted_numbers.txt"  #as per given statement


# Ensures that the output file is empty
> "$OUTPUT_FILE"  #Output  File

#  Now Read the input file line by line
while IFS=$'\t' read -r INPUT_NUM INPUT_BASE CONVERT_BASE;  
#here this IFS stands for Internal Field Separator
#IFS indicates how the words are separated on the command line
do

    # Convert the input number to the desired base according to the given problem
    CONVERTED_NUM=$(echo "obase=$CONVERT_BASE; ibase=$INPUT_BASE; $INPUT_NUM" | bc)
#obase:-Output Base which is the converted base as per the problem and Ibase is an input    

    # Append the results to the output file
    echo -e "$CONVERTED_NUM" >> "$OUTPUT_FILE"
done 
< "$INPUT_FILE" #Input File