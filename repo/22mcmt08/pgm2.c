#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    const char* input_file = "input_numbers.txt";
    const char* output_file = "output_numbers.txt";
    FILE* input = fopen(input_file, "r");
    FILE* output = fopen(output_file, "w");

    // Check if the input file exists
    if (input == NULL) {
        printf("Input file not found: %s\n", input_file);
        return 1;
    }

    char line[256];
    while (fgets(line, sizeof(line), input)) {
        int number, base_input, base_conversion;
        if (sscanf(line, "%d%d%d", &number, &base_input, &base_conversion) == 3) {
            fprintf(output, "%d\n", number);
        } else {
            printf("Invalid input number: %s", line);
        }
    }

    fclose(input);
    fclose(output);

    printf("Conversion completed. Results are stored in %s.\n", output_file);
    return 0;
}

