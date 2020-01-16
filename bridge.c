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
#include "bridge.h"

#define _XOPEN_SOURCE 700

#define FLAG_SEQUENCE                   0x7e
#define CONTROL_ESCAPE                  0x7d
#define BRIDGE_KEY_IEEE154_TXPOWER      23
#define BRIDGE_OP_GET                   1
#define WPAN_PROTO_BRIDGE               0

int fd;
wpan_hdlc_info_t hdlc_info;

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

int bridge_get_buf(uint8_t *buf, int req_key, struct bridge_value *value, uint8_t arg)
{
    struct bcontrol_hdr req_hdr;
    int size;
    req_hdr.op = BRIDGE_OP_GET;                 //bridge get
    req_hdr.len = value->len;
    req_hdr.type = req_key;
    req_hdr.version = 9;
    req_hdr.arg = arg;
    memcpy(buf, &req_hdr, sizeof(req_hdr));
    memcpy(buf + sizeof(req_hdr), &value->u, value->len);
    return sizeof(req_hdr) + value->len;
}

static uint16_t pppfcs16 (uint16_t fcs, unsigned char cp)
{
    return (fcs >> 8) ^ fcstab[(fcs ^ cp) & 0xff];
}

static uint8_t *stuff_byte (uint8_t *buf, uint8_t byte)
{
    if (byte == CONTROL_ESCAPE || byte == FLAG_SEQUENCE) {
        *buf++ = CONTROL_ESCAPE;
        byte ^= 0x20;
    }
    *buf++ = byte;
    return buf;
}

static uint8_t *stuff_byte_crc(uint8_t *buf, uint8_t byte, uint16_t *crc){
    *crc = pppfcs16(*crc, byte);
    return stuff_byte(buf, byte);
}

int hdlc_encode(uint8_t *orig_buf, uint8_t *tx_buf, int len)
{
    int tx_len;
    uint16_t crc = 0xffff;
    uint8_t *txcur = tx_buf;
    int extra_bytes = 6;

    *txcur++ = FLAG_SEQUENCE;
    txcur = stuff_byte_crc(txcur, WPAN_PROTO_BRIDGE, &crc);
    txcur = stuff_byte_crc(txcur, BRIDGE_OP_GET, &crc);

    for(int i = 0; i < len; i++){
        if(txcur - tx_buf > 4096 - extra_bytes){
            return -1;
        }
        txcur = stuff_byte_crc(txcur, *orig_buf++, &crc);
    }
    crc ^= 0xffff;
    txcur = stuff_byte(txcur, crc);
    txcur = stuff_byte(txcur, crc >> 8);
    *txcur++ = FLAG_SEQUENCE;
    return 0;
}

void sig_handler_IO(int status)
{
    uint8_t byte;
    uint8_t buf[200];
    int bytes = read(fd, buf, 200);
    hdlc_info.rx_len = bytes;
    printf("Read %d bytes ", bytes);
    if(bytes > 0){
        //printf("Received hardware version is: ");
        for(int i = 0; i < bytes; i++){
            printf("%02X ", buf[i]);
        }
        printf("\n");
    }
    for(int i = 0; i < bytes; i++)
    {
        byte = hdlc_info.rx_buf[i];   
        switch (hdlc_info.rx_state)
        {

            case HDLC_NOSYNC:
                if(byte == FLAG_SEQUENCE){
                    hdlc_info.rx_state = HDLC_SYNC;
                    hdlc_info.rx_len = 0;
                }
                break;

            case HDLC_SYNC:
                switch (byte)
                {
                    case CONTROL_ESCAPE:
                        hdlc_info.rx_state = HDLC_ESCAPED;
                        break;
                    case FLAG_SEQUENCE:
                        if(hdlc_info.rx_len){
                            //hdlc_process_rx_frame();
                            hdlc_info.rx_len = 0;
                        }
                        break;
                    default:
                        hdlc_info.rx_buf[hdlc_info.rx_len++] = byte;
                        break;
                }

            case HDLC_ESCAPED:
                hdlc_info.rx_buf[hdlc_info.rx_len++] = bytes ^ 0x20;
                hdlc_info.rx_state = HDLC_SYNC;
                break;

            default:
                perror("Unexpected hdlc rx state\n");
                break;
        } // end switch
    } // end for
    
}


void main()
{
    int bytes, len, tx_len;
    char c;
    char *uart = "/dev/ttyUSB0";
    struct bridge_value value;
    uint8_t bridge_buf[sizeof(struct bcontrol_hdr) + 1500];    
    struct sigaction saio;
    // Initialize sigaction
    saio.sa_handler = sig_handler_IO;
    saio.sa_flags = 0;
    saio.sa_restorer = NULL;
    sigaction(SIGIO, &saio, NULL);
    // Initialize hdlc_info
    memset(&hdlc_info, 0, sizeof(hdlc_info));

    if((fd = open(uart, O_RDWR|O_NOCTTY|O_NDELAY)) < 0){
        perror("open failed");
        return;
    }
    set_opt(fd, 3000000, 8, 'N', 1);
    fcntl(fd, F_SETFL, O_ASYNC);
    fcntl(fd, __F_SETOWN, getpid());
    // Construct bridge control packet
    len = bridge_get_buf(bridge_buf, 100, &value, 0);
    uint8_t test_buf[] = {0x7e, 0x7d, 0x20, 0xba, 0x7d, 0x25, 0x7d, 0x21, 0x64, 0x7d, 0x20, 0x7d, 0x20, 0x7d, 0x20, 0xb8, 0xce, 0x7e};
    hdlc_encode(bridge_buf, hdlc_info.tx_buf, len); 
    while(1){
        printf("Press Enter to send a packet\n");
        do
        {
            c = getchar();
        } while (c != '\n');
        bytes = write(fd, test_buf, 18);
        printf("Write %d bytes: ", bytes);
        for(int i = 0; i < bytes; i++)
        {
            printf("%02X ", test_buf[i]);
        }
        printf("\n");
    }
    close(fd);
    printf("Device closed\n");
}