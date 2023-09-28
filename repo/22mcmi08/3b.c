#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_COMMAND_LENGTH 1024

int main() {
    char command[MAX_COMMAND_LENGTH];
    FILE *pipe_output;

    printf("Linux Bash Command Interpreter\n");
    printf("Enter a command (ls, grep, wc, or 'exit' to quit):\n");

    while (1) {
        printf("> ");
        fgets(command, sizeof(command), stdin);

        // Remove newline character
        size_t len = strlen(command);
        if (len > 0 && command[len - 1] == '\n') {
            command[len - 1] = '\0';
        }

        // Check if the user wants to exit
        if (strcmp(command, "exit") == 0) {
            printf("Exiting the program...\n");
            break;
        }

        // Execute the command and read its output
        pipe_output = popen(command, "r");
        if (pipe_output == NULL) {
            perror("Command execution failed");
            continue;
        }

        char output_line[MAX_COMMAND_LENGTH];
        while (fgets(output_line, sizeof(output_line), pipe_output) != NULL) {
            printf("%s", output_line);
        }

        pclose(pipe_output);
    }

    return 0;
}

