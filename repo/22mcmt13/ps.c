#include <stdio.h>
#include <ctype.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

#define FORMAT "%5s %s\t%8s %s\n"

int main(int argc, char** argv) {
    DIR *dir;
    struct dirent *ent;
    int i, fd_self, fd;
    unsigned long time, stime;
    char flag, *tty;
    char cmd[256], tty_self[256], path[256], time_s[256];
    FILE* file;

    dir = opendir("/proc");
    fd_self = open("/proc/self/fd/0", O_RDONLY);
    snprintf(tty_self, sizeof(tty_self), "%s", ttyname(fd_self));
    printf(FORMAT, "PID", "TTY", "TIME", "CMD");

    while ((ent = readdir(dir)) != NULL) {
        flag = 1;
        for (i = 0; ent->d_name[i]; i++) {
            if (!isdigit(ent->d_name[i])) {
                flag = 0;
                break;
            }
        }

        if (flag) {
            snprintf(path, sizeof(path), "/proc/%s/fd/0", ent->d_name);
            fd = open(path, O_RDONLY);
            if(fd == -1) {
                continue;  // Error opening, continue to the next iteration
            }
            tty = ttyname(fd);

            if (tty && strcmp(tty, tty_self) == 0) {
                snprintf(path, sizeof(path), "/proc/%s/stat", ent->d_name);
                file = fopen(path, "r");
                if (file) {
                    fscanf(file, "%d %s %c", &i, cmd, &flag);
                    cmd[strlen(cmd) - 1] = '\0';
                    for (i = 0; i < 11; i++)
                        fscanf(file, "%lu", &time);
                    fscanf(file, "%lu", &stime);
                    time = (time + stime) / sysconf(_SC_CLK_TCK);
                    snprintf(time_s, sizeof(time_s), "%02lu:%02lu:%02lu",
                        (time / 3600) % 24, (time / 60) % 60, time % 60);
                    printf(FORMAT, ent->d_name, tty + 5, time_s, cmd + 1);
                    fclose(file);
                }
            }
            close(fd);
        }
    }
    closedir(dir);
    close(fd_self);
    return 0;
}

