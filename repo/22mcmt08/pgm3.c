#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    char command[] = "find /home/angel/Desktop/0s_lab -type f -name \"A*.txt\" | wc -l";		//Please give the path of the folder after 'find'
    
    int pipe_fd[2];
    if (pipe(pipe_fd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) 	//This is the child process executing
    {
        close(pipe_fd[0]);  
        dup2(pipe_fd[1], STDOUT_FILENO);  
        close(pipe_fd[1]);  
        system(command);  
        exit(EXIT_SUCCESS);
    } 
    else 		//This is the parent process executing
    {
        close(pipe_fd[1]);  
        char buffer[256];
        ssize_t bytes_read;

        while ((bytes_read = read(pipe_fd[0], buffer, sizeof(buffer))) > 0) {
            write(STDOUT_FILENO, buffer, bytes_read);  
        }

        close(pipe_fd[0]);
        wait(NULL);  
    }

    return 0;
}
