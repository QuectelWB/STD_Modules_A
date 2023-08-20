/********************************************************************************************************
【文件】uart.cpp
【描述】串口读写控制:适用于linux串口设备平台
【时间】2018-6-29
********************************************************************************************************/
#include <termios.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
 
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
 
#include <signal.h>
#include "uart.h"
 
/********************************************************************************************************
【函数名】getBaudrate
【功  能】返回对应波特率宏定义
【参  数】baudrate			波特率大小
【返回值】波特率宏定义
********************************************************************************************************/
static speed_t getBaudrate(int baudrate)
{
    switch(baudrate) {
    case 0: return B0;
    case 50: return B50;
    case 75: return B75;
    case 110: return B110;
    case 134: return B134;
    case 150: return B150;
    case 200: return B200;
    case 300: return B300;
    case 600: return B600;
    case 1200: return B1200;
    case 1800: return B1800;
    case 2400: return B2400;
    case 4800: return B4800;
    case 9600: return B9600;
    case 19200: return B19200;
    case 38400: return B38400;
    case 57600: return B57600;
    case 115200: return B115200;
    case 230400: return B230400;
    case 460800: return B460800;
    case 500000: return B500000;
    case 576000: return B576000;
    case 921600: return B921600;
    case 1000000: return B1000000;
    case 1152000: return B1152000;
    case 1500000: return B1500000;
    case 2000000: return B2000000;
    case 2500000: return B2500000;
    case 3000000: return B3000000;
    case 3500000: return B3500000;
    case 4000000: return B4000000;
    default: return -1;
    }
}
/********************************************************************************************************
【函数名】UART
【功  能】UART的构造函数，阻塞方式打开一个串口设备文件
【参  数】devFile	:表示UART对应的设备文件
【返回值】无
********************************************************************************************************/
UART::UART(char* devFile)
{
/*
    先以非阻塞方式打开一个串口设备文件：
    O_RDWR：可读可写
    O_NOCTTY：不以终端设备方式打开
    O_NDELAY：非阻塞方式读，无数据时直接返回0
*/
    mFd=open(devFile,O_RDWR | O_NOCTTY | O_NDELAY);
    if (mFd > 0)
    {
/*恢复串口为阻塞状态*/
        if(fcntl(mFd, F_SETFL, 0)<0)
            printf("fcntl failed!\n");
        else
            printf("fcntl=%d\n",fcntl(mFd, F_SETFL,0));
    }else{
        printf("Can't open %s\n",devFile);
        exit(1);
    }
}
UART::~UART()
{
    close(mFd);
}
/********************************************************************************************************
【函数名】UART::cfg
【功  能】配置UART工作参数
【参  数】inSpeed  ：串口波特率
         inBits   ：数据位
         inEvent  ：奇偶校验位
         inStop   ：停止位
         inReadLen: 阻塞方式一次读取字节最大长度
         inTimeout：阻塞超时等待时长，单位：inTimeout*100ms
【返回值】返回0表示配置成功；否则表示配置失败
********************************************************************************************************/
int UART::cfg(int inSpeed,int inBits,int inEvent ,int inStop, int inReadLen, int inTimeout)
{
    struct termios newtio,oldtio;
    if ( tcgetattr (mFd,&oldtio)  !=  0 ){
        perror("Setup Serial");
        return -1;
    }
    bzero( &newtio, sizeof( newtio ) );
    newtio.c_cflag |=  CLOCAL|CREAD;
    newtio.c_cflag &= ~CSIZE;
    switch(inBits){
//设置数据位
    case 7:
        newtio.c_cflag |= CS7;
        break;
    case 8:
        newtio.c_cflag |= CS8;
        break;
    }
//设置奇偶校验位
    switch(inEvent){
    case 'O':
        newtio.c_cflag |=PARENB;
        newtio.c_cflag |=PARODD;
        newtio.c_iflag |=(INPCK|ISTRIP);
        break;
    case 'E':
        newtio.c_iflag |=(INPCK|ISTRIP);
        newtio.c_cflag |=PARENB;
        newtio.c_cflag &=~PARODD;
        break;
    case 'N':
        newtio.c_cflag &=~PARENB;
        break;
    }
//设置波特率
    cfsetispeed(&newtio, getBaudrate(inSpeed));
    cfsetospeed(&newtio, getBaudrate(inSpeed));
//停止位设置
    if(inStop==1){
        newtio.c_cflag &= ~CSTOPB;
    }else if(inStop==2){
        newtio.c_cflag |= CSTOPB;
    }
/* 阻塞读取字节设置：
    每读取到inReadLen个字节后read函数返回，
    或者是在接收到不够inReadLen字节时，
    等待时长超过inTimeout*100ms时函数返回
 */
    newtio.c_cc[VTIME]  = inTimeout;
    newtio.c_cc[VMIN] 	= inReadLen;
    tcflush(mFd,TCIFLUSH);
 
    if((tcsetattr(mFd,TCSANOW,&newtio))!=0){
        perror("Set uart error");
        return -1;
    }
 
    printf("Set uart done\n");
    return 0;
}
int UART::mflush(void)
{
    return tcflush(mFd,TCIFLUSH);
}
/********************************************************************************************************
【函数名】UART::write
【功  能】往串口设备发送数据
【参  数】pcnBuf ：数据缓冲区
         inLen	：数据缓冲区长度
【返回值】返回0表示写入成功；否则表示写入失败
********************************************************************************************************/
int UART::mwrite(char *pcnBuf, int inLen)
{
    int nw;
    nw = write(mFd, pcnBuf, inLen) ;
    if (nw == inLen)
    {
        return 0;
    }else
    {
        printf("Error write inLen = %d,nw = %d\n",inLen,nw);
        return -1;
    }return -1;
}
 
/********************************************************************************************************
【函数名】UART::read
【功  能】往串口设备读取数据
【参  数】pcnBuf ：数据缓冲区
         inLen  ：数据缓冲区长度
【返回值】返回0表示读取成功；否则读取失败
********************************************************************************************************/
int UART::mread(char *pcnBuf, int inLen)
{
    int nr;
    nr = read(mFd, pcnBuf, inLen);
    tcflush(mFd,TCIFLUSH);
    if (nr > inLen)
    {
        return 0;
    }else
    {
        printf("Error read inLen = %d,nw = %d\n",inLen,nr);
        return -1;
    }return -1;
}
 
/********************************************************************************************************
【函数名】UART::mselect
【功  能】以select机制等待数据
【参  数】inTimeoutMs ：等待时长
【返回值】0:表示等待超时,-1:执行select失败,>0:数据可读
********************************************************************************************************/
int UART::mselect(int inTimeoutMs)
{
    int retval;
    FD_ZERO(&mRd);
    FD_SET(mFd,&mRd);
    mTimeout.tv_sec = inTimeoutMs/1000;
    mTimeout.tv_usec = (inTimeoutMs%1000)*1000;
 
    retval = select(mFd+1, &mRd, NULL, NULL, &mTimeout);
    return (retval);
}

