网络编程接口
=========

|接口|ioctl请求|数据类型|
|---|---| ---|
|SIOCGIFCONF<br>SIOCSIFADDR<br>SIOCGIFADDR<br>SIOCSIFBRDADDR<br>SIOCGIFBRDADDR<br>SIOCSIFNETMASK<br>SIOCGIFNETMASK | 获取所有接口列表<br>设置接口地址<br>获取接口地址<br>设置广播地址<br>获取广播地址<br>设置子网掩码<br>获取子网掩码 | Struct  ifconf <br> Struct  ifreq <br> Struct  ifreq <br> Struct  ifreq<br> Struct  ifreq<br> Struct  ifreq <br> Struct  ifreq |

<br>

	
	struct ifconf{
	lint ifc_len;
	union{
	    	caddr_t  ifcu_buf
	  	struct   ifreq *ifcu_req;
	     }ifc_ifcu
	}
	#define    ifc_buf    ifc_ifcu.ifcu_buf
	#define    ifc_req    ifc_ifcu.ifcu_req

	struct ifreq{
	char ifr_name[IFNAMSIZ];
	Union{
		    struct  sockaddr  ifru_addr;
		    struct  sockaddr  ifru_dstaddr;
		    struct  sockaddr  ifru_broadaddr;
		    struct  sockaddr  ifru_netmask;
		    struct  sockaddr  ifru_hwaddr;
		    short  ifru_flags;
		    int     ifru_metric;
		    caddr_t ifru_data;
	     }ifr_ifru;
	};

	#define ifr_addr        ifr_ifru.ifru_addr
	#define ifr_broadaddr   ifr_ifru.ifru_broadadd
	#define ifr_hwaddr      ifr_ifru_hwaddr

ifconf中ifc_buf,就是N个ifc_req.


C代码的例子

	#include<stdlib.h>  
	#include<stdio.h>  
	#include<unistd.h>  
	#include<net/if.h>  
	#include<net/if_arp.h>  
	#include<arpa/inet.h>  
	#include<sys/ioctl.h>  
	#include<errno.h>  
	   
	#define ETH_NAME  "eth0"  
	   
	static void get_local_ip(int sock_fd);  
	static void get_netmask_addr(int sock_fd);  
	static void get_mac_addr(int sock_fd);  
	static void get_broad_addr(int sock_fd);  
	struct sockaddr_in  sin;  
	struct sockaddr_in  netmask;  
	struct sockaddr_in  broad;  
	struct ifreq ifr;  
	unsigned char arp[6];  
	
	int main()  
	{  
		int sockfd;  
		//create socket  
		sockfd = socket(AF_INET,SOCK_DGRAM,0);  
		
		if(sockfd == -1)  
		{  
			perror("socket");  
			return -1;  
		}
		
		get_local_ip(sockfd);  
		get_mac_addr(sockfd);  
		get_netmask_addr(sockfd);  
		get_broad_addr(sockfd);  
		return 0;  
	}  
		
	static void get_local_ip(int sock_fd)  
	{  
		strncpy(ifr.ifr_name,ETH_NAME,IFNAMSIZ);  
		ifr.ifr_name[IFNAMSIZ - 1] = 0;  
		if(ioctl(sock_fd,SIOCGIFADDR,&ifr) == 0)  
		{  
		memcpy(&sin,&ifr.ifr_addr,sizeof(sin));  
		fprintf(stdout,"eth0: %s\n",inet_ntoa(sin.sin_addr));  
		}  
	} 
	 
	static void get_mac_addr(int sock_fd)  
	{  
		if(ioctl(sock_fd,SIOCGIFHWADDR,&ifr)==0)  
		{  
		memcpy(arp,ifr.ifr_hwaddr.sa_data,6);  
		printf("adapter hardware address %x:%x:%x:%x:%x:%x\n",arp[0],arp[1],arp[2],arp[3],arp[4],arp[5]);  
		}  
	}

	static void get_netmask_addr(int sock_fd)  
	{  
		if(ioctl(sock_fd,SIOCGIFNETMASK,&ifr)==0)  
		{  
			memcpy(&netmask,&ifr.ifr_netmask,sizeof(netmask));  
			printf("netmask: %s\n",inet_ntoa(netmask.sin_addr));  
		}  
	} 

	static void get_broad_addr(int sock_fd)  
	{  
		if(ioctl(sock_fd,SIOCGIFBRDADDR,&ifr)==0)  
		{  
			memcpy(&broad,&ifr.ifr_broadaddr,sizeof(broad));  
			printf("broad: %s\n",inet_ntoa(broad.sin_addr));  
		}  
	}

	v@WSL:~/Blogs/STD_Modules_A/Performance/Coding$ sudo ./test_if
	[sudo] password for v:
	wifi0: 10.66.25.73
	adapter hardware address 78:92:9c:e8:38:a2
	netmask: 255.255.252.0
	broad: 10.66.27.255
