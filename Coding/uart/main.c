#include "uart.h"
#include <unistd.h>
//回环测试
int main(void)
{
	int retval;
	UART *mCom = new UART("/dev/ttyUSB6");

	char str[] = "This is a example project";
	char recBuf[50];
	
    	mCom->cfg(115200,8,'N',1,50,1);
	while(1)
	    {
		retval = mCom->mwrite(str,strlen(str)+1);
		if (retval == 0)
		{
		    retval = mCom->mselect(1000);
		    if (retval > 0)
		    {
			retval = mCom->mread(recBuf,50);
			if (retval == 0)
			{
			    printf("Rec Str = %s\n",recBuf);
			}else
			{
			    printf("Read error:%d",retval);
			}
		    }else
		    {
			printf("Timeout:%d",retval);
		    }
		}else
		{
		    printf("Write error:%d",retval);
        	}
        	sleep(1);
    }
}
