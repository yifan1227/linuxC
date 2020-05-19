#include <stdio.h>                                                             
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <string.h>
 
#define BUFS PIPE_BUF


int main()
{
    int fd;
    int n, i;
    char buf[BUFS];
    const char *fifo = "/home/y/cgr-lite/cli.fifo";
    time_t tp;
 
    printf("I am %d\n", getpid());
    if (access(fifo, F_OK) != 0)
    {
        printf("cli.fifo not existed\n");
        exit(1);
    }
    if ((fd = open(fifo, O_WRONLY)) < 0)
    {
        printf("Open failed!\n");
        exit(1);
    }
    
    char cmd[50];
    int len;

    while(1){
        fgets(cmd, 60, stdin);
        len = strlen(cmd);        
        write(fd, cmd, len);
    }
    //for (i = 0; i < 10; i++)
    //{
        time(&tp);
        n = sprintf(buf, "write_fifo %d sends %s", getpid(), ctime(&tp));
        printf("Send msg:%s", buf);
        if ((write(fd, buf, n + 1)) < 0)
        {
            printf("Write failed!\n");
            close(fd);
            exit(1);
        }
        sleep(100);
    //}
    close(fd);
    exit(0);
}