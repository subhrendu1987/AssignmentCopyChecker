#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    while (1) {
        char command[100];
        printf("Enter a command (ps, echo, kill, sleep, or exit): ");
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
        if (strcmp(tokens[0], "ps") == 0) {
            // Execute 'ps' command
            pid_t pid = fork();
            if (pid == 0) {
                execvp("ps", tokens);
                perror("execvp ps failed");
                exit(1);
            } else {
                wait(NULL);
            }
        } else if (strcmp(tokens[0], "echo") == 0) {
            // Execute 'echo' command
            for (int i = 1; i < num_tokens; i++) {
                printf("%s ", tokens[i]);
            }
            printf("\n");
        } else if (strcmp(tokens[0], "kill") == 0) {
            // Execute 'kill' command
            if (num_tokens != 3) {
                printf("Usage: kill <SIGNAL> <PID>\n");
            } else {
                int signal = atoi(tokens[1]);
                pid_t pid = atoi(tokens[2]);
                kill(pid, signal);
            }
        } else if (strcmp(tokens[0], "sleep") == 0) {
            // Execute 'sleep' command
            if (num_tokens != 2) {
                printf("Usage: sleep <SECONDS>\n");
            } else {
                int seconds = atoi(tokens[1]);
                sleep(seconds);
            }
        } else if (strcmp(tokens[0], "exit") == 0) {
            // Exit the program
            break;
        } else {
            printf("Command not recognized: %s\n", tokens[0]);
        }
    }

    return 0;
}

