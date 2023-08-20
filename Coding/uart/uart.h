#ifndef _UART_H__
#define _UART_H__
/********************************************************************************************************
【类名】UART
【描述】实现阻塞方式不定长读取数据。不定长阻塞方式写入数据
【属性】
     devFile:GPIO对应的设备文件。
     fd:GPIO设备文件的文件描述符。
【函数】
     UART:	UART类构造函数，打开对应设备文件
     cfg:				配置UART
     write:			写串口
     read:			读串口
********************************************************************************************************/
//#include "common.h"
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <stdio.h>

class UART
{
public:
    UART(char* cnDevFile);
    ~UART();
    int cfg(int inSpeed,int inBits,int inEvent ,int inStop, int inReadLen, int inTimeout);
    int mwrite(char* pcnBuf, int inLen);
    int mread(char* pcnBuf, int inLen);
    int mselect(int inTimeoutMs);
    int mflush(void);
private:
    int mFd;
    fd_set mRd;
    struct timeval mTimeout;
};

#endif
