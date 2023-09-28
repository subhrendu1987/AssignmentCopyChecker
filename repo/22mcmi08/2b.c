#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    while (1) {
        char command[100];
        printf("Enter a command (echo, exit): ");
        fgets(command, sizeof(command), stdin);

        // Remove newline character
        size_t len = strlen(command);
        if (len > 0 && command[len - 1] == '\n') {
            command[len - 1] = '\0';
        }

        // Split the input into tokens
        char *tokens[10];
        int num_tokens = 0;
        char *token = strtok(command, " ");
        while (token != NULL) {
            tokens[num_tokens++] = token;
            token = strtok(NULL, " ");
        }
        tokens[num_tokens] = NULL;

        // Execute the specified command
        if (strcmp(tokens[0], "echo") == 0) {
            // Execute 'echo' command
            for (int i = 1; i < num_tokens; i++) {
                printf("%s ", tokens[i]);
            }
            printf("\n");
        } else if (strcmp(tokens[0], "exit") == 0) {
            // Exit the program
            break;
        } else {
            printf("Command not recognized: %s\n", tokens[0]);
        }
    }

    return 0;
}

