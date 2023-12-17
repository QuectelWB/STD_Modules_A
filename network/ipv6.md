# ipv6


### Linux 内核什么时候会发RS

inet6_addr_add >> addrconf_dad_start >> addrconf_dad_completed >> ndisc_send_rs

第一次发送RS，并设置定时器和发包次数，这个也可以通过proc配置



### 内核是怎么收 RA 

ndisc_rcv >> ndisc_router_discovery

解析option信息，标志位，根据前缀配置路由

vlan2 接口在起接口和配置本地链路地址时发送，走addrconf_add_linklocal




### 其他

https://blog.csdn.net/sinat_27261621/article/details/90607031


    net.ipv6.conf.default.accept_ra=2
    net.ipv6.conf.all.accept_ra=2
    net.ipv6.conf.default.use_tempaddr=0
    net.ipv6.conf.all.use_tempaddr=0


    
