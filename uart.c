#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <error.h>
#include <stdint.h>

#define FLAG_SEQUENCE                   0x7e
#define CONTROL_ESCAPE                  0x7
#define BRIDGE_KEY_IEEE154_TXPOWER      23
#define BRIDGE_OP_GET                   1
#define WPAN_PROTO_BRIDGE               0

struct bcontrol_hdr{    
    uint8_t version;
    uint8_t op;
    uint8_t type;
    uint8_t arg;
    uint16_t len;
};

struct bridge_value{
    uint16_t len;
    union{
        uint8_t data8;
        uint16_t data16;
        uint32_t data32;
        uint8_t data[1500];
    }u;
};

static uint16_t fcstab[256] = {
    0x0000, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf,
    0x8c48, 0x9dc1, 0xaf5a, 0xbed3, 0xca6c, 0xdbe5, 0xe97e, 0xf8f7,
    0x1081, 0x0108, 0x3393, 0x221a, 0x56a5, 0x472c, 0x75b7, 0x643e,
    0x9cc9, 0x8d40, 0xbfdb, 0xae52, 0xdaed, 0xcb64, 0xf9ff, 0xe876,
    0x2102, 0x308b, 0x0210, 0x1399, 0x6726, 0x76af, 0x4434, 0x55bd,
    0xad4a, 0xbcc3, 0x8e58, 0x9fd1, 0xeb6e, 0xfae7, 0xc87c, 0xd9f5,
    0x3183, 0x200a, 0x1291, 0x0318, 0x77a7, 0x662e, 0x54b5, 0x453c,
    0xbdcb, 0xac42, 0x9ed9, 0x8f50, 0xfbef, 0xea66, 0xd8fd, 0xc974,
    0x4204, 0x538d, 0x6116, 0x709f, 0x0420, 0x15a9, 0x2732, 0x36bb,
    0xce4c, 0xdfc5, 0xed5e, 0xfcd7, 0x8868, 0x99e1, 0xab7a, 0xbaf3,
    0x5285, 0x430c, 0x7197, 0x601e, 0x14a1, 0x0528, 0x37b3, 0x263a,
    0xdecd, 0xcf44, 0xfddf, 0xec56, 0x98e9, 0x8960, 0xbbfb, 0xaa72,
    0x6306, 0x728f, 0x4014, 0x519d, 0x2522, 0x34ab, 0x0630, 0x17b9,
    0xef4e, 0xfec7, 0xcc5c, 0xddd5, 0xa96a, 0xb8e3, 0x8a78, 0x9bf1,
    0x7387, 0x620e, 0x5095, 0x411c, 0x35a3, 0x242a, 0x16b1, 0x0738,
    0xffcf, 0xee46, 0xdcdd, 0xcd54, 0xb9eb, 0xa862, 0x9af9, 0x8b70,
    0x8408, 0x9581, 0xa71a, 0xb693, 0xc22c, 0xd3a5, 0xe13e, 0xf0b7,
    0x0840, 0x19c9, 0x2b52, 0x3adb, 0x4e64, 0x5fed, 0x6d76, 0x7cff,
    0x9489, 0x8500, 0xb79b, 0xa612, 0xd2ad, 0xc324, 0xf1bf, 0xe036,
    0x18c1, 0x0948, 0x3bd3, 0x2a5a, 0x5ee5, 0x4f6c, 0x7df7, 0x6c7e,
    0xa50a, 0xb483, 0x8618, 0x9791, 0xe32e, 0xf2a7, 0xc03c, 0xd1b5,
    0x2942, 0x38cb, 0x0a50, 0x1bd9, 0x6f66, 0x7eef, 0x4c74, 0x5dfd,
    0xb58b, 0xa402, 0x9699, 0x8710, 0xf3af, 0xe226, 0xd0bd, 0xc134,
    0x39c3, 0x284a, 0x1ad1, 0x0b58, 0x7fe7, 0x6e6e, 0x5cf5, 0x4d7c,
    0xc60c, 0xd785, 0xe51e, 0xf497, 0x8028, 0x91a1, 0xa33a, 0xb2b3,
    0x4a44, 0x5bcd, 0x6956, 0x78df, 0x0c60, 0x1de9, 0x2f72, 0x3efb,
    0xd68d, 0xc704, 0xf59f, 0xe416, 0x90a9, 0x8120, 0xb3bb, 0xa232,
    0x5ac5, 0x4b4c, 0x79d7, 0x685e, 0x1ce1, 0x0d68, 0x3ff3, 0x2e7a,
    0xe70e, 0xf687, 0xc41c, 0xd595, 0xa12a, 0xb0a3, 0x8238, 0x93b1,
    0x6b46, 0x7acf, 0x4854, 0x59dd, 0x2d62, 0x3ceb, 0x0e70, 0x1ff9,
    0xf78f, 0xe606, 0xd49d, 0xc514, 0xb1ab, 0xa022, 0x92b9, 0x8330,
    0x7bc7, 0x6a4e, 0x58d5, 0x495c, 0x3de3, 0x2c6a, 0x1ef1, 0x0f78
};

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

    newtio.c_cc[VTIME] = 0;
    newtio.c_cc[VMIN] = 10;
    //newtio.c_oflag &= ~OPOST;
    //newtio.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); 
    tcflush(fd, TCIFLUSH);

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

void main()
{
    int fd, bytes, len, tx_len;
    char *uart = "/dev/ttyUSB1";
    struct bridge_value value;
    uint8_t bridge_buf[sizeof(struct bcontrol_hdr) + 1500];
    uint8_t recv_buf[100];
    uint8_t tx_buf[50];

    memset(bridge_buf, 0, 1500);
    if((fd = open(uart, O_RDWR|O_NOCTTY|O_NDELAY)) < 0){
        perror("open failed");
        return;
    }
    printf("Open success!\n");
    set_opt(fd, 3000000, 8, 'N', 1);
    if(fcntl(fd, F_SETFL, 0) < 0)
        perror("fcntl failed");
    len = bridge_get_buf(bridge_buf, 100, &value, 0);
    uint8_t test_buf[] = {0x7e, 0x7d, 0x20, 0xba, 0x7d, 0x25, 0x7d, 0x21, 0x64, 0x7d, 0x20, 0x7d, 0x20, 0x7d, 0x20, 0xb8, 0xce, 0x7e};
    hdlc_encode(bridge_buf, tx_buf, len); 
    //hdlc_encode(test_buf, tx_buf, 6); 
    while(1){
        memset(recv_buf, 0, 1500);
        bytes = write(fd, test_buf, 18);
        printf("Write %d bytes\n", bytes);
        printf("Sent paket is: ");
        for(int i = 0; i < bytes; i++)
        {
            printf("%02X ", tx_buf[i]);
        }
        printf("\n");
        bytes = read(fd, recv_buf, 1500);
        printf("Read %d bytes\n", bytes);
        if(bytes > 0)
        {
            printf("Received packet is: ");
            for(int i = 0; i < bytes; i++)
            {
                printf("%02X ", recv_buf[i]);
            }
            printf("\n");
        }
        if(bytes < 0)
            return;
        sleep(1);
    }
    close(fd);
    printf("Device closed\n");
}