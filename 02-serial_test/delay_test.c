
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <string.h>

#define BUFLEN 24

inline int timesub_usec(struct timeval *tt, long usec)
{
    if ( tt->tv_usec > usec )
    {
        tt->tv_usec -= usec;
    }
    else
    {
        tt->tv_usec = usec - tt->tv_usec;
        --(tt->tv_sec);
    }
    return 0;
}

inline int timeadd_usec(struct timeval *tt, long usec)
{
    tt->tv_usec += usec;
    if(tt->tv_usec > 1E6)
    {
        ++(tt->tv_sec);
        tt->tv_usec -= (long)1E6;
    }
    return 0;
}

int serial_open(const char *port)
{
    int fd;

    if ((fd = open(port, O_RDWR | O_NOCTTY | O_NDELAY)) < 0)
    {
        perror("Can't Open Serial Port");
        return -1;
    }
    if (fcntl(fd, F_SETFL, 0) < 0)
    {
        perror("fcntl failed");
        return -1;
    }

    return fd;
}
 
int serial_init(int fd)  
{  
    struct termios options;

    //get attr
    if ( tcgetattr(fd, &options) != 0)
    {
        perror("Setup Serial 1");
        return -1;
    }

    //setup baudrate 115200
    cfsetispeed(&options, B115200); //input stream speed
    cfsetospeed(&options, B115200); //output stream speed

    options.c_cflag |= CLOCAL; //permission: local usage
    options.c_cflag |= CREAD; //permission: read data
    options.c_cflag &= ~CRTSCTS; //no flow control, RTS/CTS

    //Setup data bit,  8
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;
    //Setup parity bit, even parity check
    options.c_cflag |= PARENB;         
    options.c_cflag &= ~PARODD;         
    options.c_iflag |= INPCK; 
    //Setup stop bit, 1
    options.c_cflag &= ~CSTOPB; // stopbit = 1
	//raw data output
	options.c_oflag &= ~OPOST;
	options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    options.c_cc[VTIME] = 1; //read byte wait at least 1*(1/10) second
    options.c_cc[VMIN] = 1; //read 1 byte at least each time
    tcflush(fd, TCIFLUSH);

    //set attr
    if (tcsetattr(fd, TCSANOW, &options) != 0)
    {
        perror("Serial Set Error");
        return -1;
    }

    return 1;
}  

int serial_recv(int fd, char *recv_buf, int len)
{
    int recv_len, fs_sel;
    fd_set fs_read;

    FD_ZERO(&fs_read);
    FD_SET(fd, &fs_read);
    fs_sel = select(fd+1, &fs_read, NULL, NULL, NULL);
    
    if(fs_sel)
    {
        recv_len = read(fd, recv_buf, len);
        return recv_len;
    }
    
    return -1;
}

int main(int argc, char const *argv[])
{
    int ret, fd;
    int recv_len;
    struct timeval t0, t1, res;
    char buf[BUFLEN];

    if ( (fd = serial_open("/dev/ttyUSB0")) < 0 )
    {
        perror("No serial port exists.");
        exit(1);
    }
    else
    {
        serial_init(fd);
    }
    
    while(1)
    {
        if ( (recv_len=serial_recv(fd, buf, sizeof("1"))) >= 0)
        {
            gettimeofday(&t0, NULL);
        }
        
        if ( (recv_len=serial_recv(fd, buf, sizeof("1"))) >= 0)
        {
            gettimeofday(&t1, NULL);
        }

        timersub(&t1, &t0, &res);
        printf("%d s, %d ms, %d us\n", res.tv_sec, res.tv_usec/1000, res.tv_usec%1000);
    }
    
    return 0;
}
