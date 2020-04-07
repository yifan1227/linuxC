#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <error.h>
#include <stdint.h>
#include <signal.h>

#define _XOPEN_SOURCE 700

int fd;

int set_opt(int fd, int nSpeed, int nBits, char nEvent, int nStop)
{
    struct termios oldtio, newtio;
    if(tcgetattr(fd, &oldtio) != 0){
        perror("Setup serial");
        return -1;
    }
    memset(&newtio, 0, sizeof(newtio));
    newtio.c_cflag |= CLOCAL | CREAD;
    newtio.c_cflag &= ~CSIZE;

    switch (nBits)
    {
    case 7:
        newtio.c_cflag |= CS7;
        break;
    case 8:
        newtio.c_cflag |= CS8;
        break;
    }

    switch (nEvent)
    {
    case 'O':
        newtio.c_cflag |= PARENB;
        newtio.c_cflag |= PARODD;
        newtio.c_iflag |= (INPCK | ISTRIP);
        break;
    case 'E':
        newtio.c_iflag |= (INPCK | ISTRIP);
        newtio.c_cflag |= PARENB;
        newtio.c_cflag &= ~PARODD;
        break;
    case 'N':
        newtio.c_cflag &= ~PARENB;
        break;
    }

    switch (nSpeed)
    {
    case 9600:
        cfsetispeed(&newtio, B9600);
        cfsetospeed(&newtio, B9600);
        break;
    case 115200:
        cfsetispeed(&newtio, B115200);
        cfsetospeed(&newtio, B115200);
        break; 
    case 3000000:
        cfsetispeed(&newtio, B3000000);
        cfsetospeed(&newtio, B3000000);
        break;
    }
    if(nStop == 1)
        newtio.c_cflag &= ~CSTOPB;
    else if(nStop == 2){
        newtio.c_cflag |= CSTOPB;
    }
    //newtio.c_cc[VTIME] = 0;
    newtio.c_cc[VMIN] = 1;
    tcflush(fd, TCIOFLUSH);

    if((tcsetattr(fd, TCSANOW, &newtio)) != 0)
    {
        perror("COM set error");
    }
    printf("COM set success!\n");
    return 0;
}

void sig_handler_IO(int status)
{
    uint8_t byte;
    uint8_t buf[200];
    int bytes = read(fd, buf, 200);
    printf("Read %d bytes ", bytes);
    if(bytes > 0){
        for(int i = 0; i < bytes; i++){
            printf("%02X ", buf[i]);
        }
        printf("\n");
    }
}


void main()
{
    int bytes, len, tx_len;
    char c;
    char *uart = "/dev/ttyUSB4";
    struct sigaction saio;
    // Initialize sigaction
    saio.sa_handler = sig_handler_IO;
    saio.sa_flags = 0;
    saio.sa_restorer = NULL;
    sigaction(SIGIO, &saio, NULL);

    if((fd = open(uart, O_RDWR|O_NOCTTY|O_NDELAY)) < 0){
        perror("open failed");
        return;
    }
    set_opt(fd, 3000000, 8, 'N', 1);
    fcntl(fd, F_SETFL, O_ASYNC);
    fcntl(fd, __F_SETOWN, getpid());
    // Construct bridge control packet
    while(1){
        printf("Press Enter to send a packet\n");
        do
        {
            c = getchar();
        } while (c != '\n');
        bytes = write(fd, NULL, 12);
    }
    close(fd);
    printf("Device closed\n");
}