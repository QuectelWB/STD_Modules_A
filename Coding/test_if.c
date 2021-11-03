#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<net/if.h>
#include<net/if_arp.h>
#include<arpa/inet.h>
#include<sys/ioctl.h>
#include<errno.h>
#include <string.h>
#include <sys/socket.h>
#define ETH_NAME  "wifi0"

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
                fprintf(stdout,"wifi0: %s\n",inet_ntoa(sin.sin_addr));
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

