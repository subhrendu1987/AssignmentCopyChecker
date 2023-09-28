#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void count_files_with_A_and_txt() {
    // Set up pipes
    int pipe_fd[2];
    if (pipe(pipe_fd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Fork a child process
    pid_t child_pid = fork();

    if (child_pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (child_pid == 0) {  // Child process
        // Close the read end of the pipe
        close(pipe_fd[0]);

        // Redirect stdout to the write end of the pipe
        dup2(pipe_fd[1], STDOUT_FILENO);
        close(pipe_fd[1]);

        // Execute the shell command
        execlp("ls", "ls", "-1", "A*.txt", NULL);

        // If execlp fails
        perror("execlp");
        exit(EXIT_FAILURE);
    } else {  // Parent process
        // Close the write end of the pipe
        close(pipe_fd[1]);

        // Redirect stdin to the read end of the pipe
        dup2(pipe_fd[0], STDIN_FILENO);
        close(pipe_fd[0]);

        // Execute the shell command 'wc -l'
        execlp("wc", "wc", "-l", NULL);

        // If execlp fails
        perror("execlp");
        exit(EXIT_FAILURE);
    }
}

int main() {
    count_files_with_A_and_txt();
    return 0;
}