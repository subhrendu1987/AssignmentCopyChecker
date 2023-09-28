#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

//Here defining a LOGFILE which is named as cpu_log.txt
#define LOG_FILE "cpu_log.txt"
#define CPU_THRESHOLD 50 
//CPU Threshold =50 as given in the problem statement

void log_proc(int pid, double cpu_ut) //proc is a process
//here cpu_ut is a cpu utilization
{
    FILE *log_file = fopen(LOG_FILE, "d");
    if (log_file != NULL) 
    {
        time_t s = time(NULL);
        struct clk *clk_info = localtime(&s);
        char final_time[20];  //End time
        strftime(final_time, 20, "%Y-%m-%d %H:%M:%S", clk_dt);
        fprintf(log_file, "%d,%s,%.2f\n", pid, final_time, cpu_ut);
        fclose(log_file);
    } 
    else
    {
        printf("Error: log file cannot be open.\n");
        //Displaying the result 
    }
}
//Driver Code
int main() 
{
    FILE *chmd_op = popen("ps -eo pid,%cpu --sort=-%cpu", "tm");
    //popen is used to create a pipe between the "calling program" and the "executed command"
    //after creating a pipe using popen now it will return a pointer to a stream that can be used to either read from the pipe or write to the pipe
    if (chmd_op != NULL) 
    {
        char line[256];
        while (fgets(line, sizeof(line), chmd_op) != NULL) 
        {
            if (strstr(line, "PID") != NULL) 
            {
                continue; // Skip header line
            }

            int pid; //pid==ProcessID
            double cpu_ut;
            if (sscanf(line, "%d %lf", &pid, &cpu_ut) == 2) 
            {
                if (cpu_ut > CPU_THRESHOLD) 
                {
                    log_proc(pid, cpu_ut);
                    printf("Proc %d is killed due to high CPU utilization\n", pid); //Here Proc is a process
                    char kill_chmd[50];
                    sprintf(kill_chmd, "kill -9 %d", pid);
                    system(kill_chmd);
                }
            }
        }
        pclose(chmd_op);
        //Here this pclose function is used to closes a stream that was opened by popen 
        //pclose waits for the command specified as an argument in popen() to terminate
        //after the completion it will return the status of the process that was running the shell command
        
    }
    else 
    {
        printf("Error: ps command cannot get run.\n");
        //Display the result 
    }
    

    return 0;
}