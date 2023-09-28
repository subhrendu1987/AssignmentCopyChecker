#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE 256
#define CPU_THRESHOLD 50.0
#define LOG_FILE "cpu_usage_log.txt"

void log_process_info(int pid, double cpu_utilized) {
    FILE *log_file = fopen(LOG_FILE, "a");
    if (log_file == NULL) {
        perror("Error opening log file");
        exit(EXIT_FAILURE);
    }

    fprintf(log_file, "%d,%.2lf%%\n", pid, cpu_utilized);
    fclose(log_file);
}

void check_and_kill_high_cpu_processes() {
    char buffer[MAX_BUFFER_SIZE];
    while (1) {
        // Run the ps command to get a list of processes and their CPU usage
        FILE *ps_output = popen("ps -eo pid,%cpu --sort=-%cpu", "r");
        if (ps_output == NULL) {
            perror("Error running ps command");
            exit(EXIT_FAILURE);
        }

        while (fgets(buffer, MAX_BUFFER_SIZE, ps_output) != NULL) {
            if (strstr(buffer, "PID") != NULL)
                continue;  // Skip the header line

            int pid;
            double cpu_usage;
            if (sscanf(buffer, "%d %lf", &pid, &cpu_usage) != 2)
                continue;  // Skip lines without PID and CPU usage information

            // Check if the CPU usage exceeds the threshold
            if (cpu_usage > CPU_THRESHOLD) {
                printf("Process %d is using %.2lf%% CPU. Killing the process...\n", pid, cpu_usage);

                // Kill the process
                char kill_command[MAX_BUFFER_SIZE];
                snprintf(kill_command, MAX_BUFFER_SIZE, "kill -9 %d", pid);
                system(kill_command);

                // Log the process information
                log_process_info(pid, cpu_usage);
            }
        }

        pclose(ps_output);

        // Sleep for 5 seconds before checking again
        sleep(5);
    }
}

int main() {
    // Create or clear the log file
    FILE *log_file = fopen(LOG_FILE, "w");
    if (log_file == NULL) {
        perror("Error creating log file");
        return EXIT_FAILURE;
    }
    fclose(log_file);

    // Start monitoring processes
    check_and_kill_high_cpu_processes();

    return 0;
}

