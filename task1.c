#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

const int STRING_SIZE = 200;

void check_current_time(char* into_string)
{
    time_t now_time;
    time(&now_time);
    struct tm* now_tm = localtime(&now_time);
    struct timespec now_timespec;
    clock_gettime(CLOCK_MONOTONIC, &now_timespec);
    long milliseconds = now_timespec.tv_nsec / 1000000;

    snprintf(into_string, STRING_SIZE, "%d:%d:%d:%ld", now_tm->tm_hour, now_tm->tm_min, now_tm->tm_sec, milliseconds);
}

void print_pid_info(pid_t pid)
{
    char time_string[STRING_SIZE];
    if (pid == 0)
    {
        check_current_time(time_string);
        printf("This is child process, pid=%d. %s\n", getpid(), time_string);
        check_current_time(time_string);
        printf("---pid of it's parent process=%d %s\n", getppid(), time_string);
    }
    else if (pid < 0)
    {
        printf("Can't invoke fork(), the process was not created\n");
    }
}
int main()
{
    pid_t pid1 = fork();
    print_pid_info(pid1);

    if (pid1 > 0)
    {
        pid_t pid2 = fork();

        if (pid2 > 0) {
            char time_string[STRING_SIZE];
            check_current_time(time_string);
            printf("This is parent process pid=%d %s\n", getpid(), time_string);
        }

        print_pid_info(pid2);
        if (pid2 > 0)
        {
            system("ps -x");
        }
    }
    return 0;
}
