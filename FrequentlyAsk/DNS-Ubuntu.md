Ubuntu 上的DNS
=====

## systemd-resolve 

https://blog.csdn.net/u011700186/article/details/109452561

一个文档里写可以 添加DNS
实际使用没有效果
	systemd-resolve --interface eth0 --set-dns 8.8.8.8 --set-dns 114.114.114.114

### 查看DNS服务状态

> systemd-resolve --status
<br>
示例：
	
	$ systemd-resolve --status
	Global
		  DNSSEC NTA: 10.in-addr.arpa
			      16.172.in-addr.arpa
			      168.192.in-addr.arpa
			      17.172.in-addr.arpa
			      18.172.in-addr.arpa
			      19.172.in-addr.arpa
			      20.172.in-addr.arpa
			      21.172.in-addr.arpa
			      22.172.in-addr.arpa
			      23.172.in-addr.arpa
			      24.172.in-addr.arpa
			      25.172.in-addr.arpa
			      26.172.in-addr.arpa
			      27.172.in-addr.arpa
			      28.172.in-addr.arpa
			      29.172.in-addr.arpa
			      30.172.in-addr.arpa
			      31.172.in-addr.arpa
			      corp
			      d.f.ip6.arpa
			      home
			      internal
			      intranet
			      lan
			      local
			      private
			      test
	Link 2 (ens3)
	      Current Scopes: DNS
	       LLMNR setting: yes
	MulticastDNS setting: no
	      DNSSEC setting: no
	    DNSSEC supported: no
		 DNS Servers: 192.168.0.1

2、解析域名的的 IP

systemd-resolve DOMAIN
示例：

jiangzl@jiangzl:~$ systemd-resolve baidu.com
baidu.com: 220.181.38.148
           39.156.69.79
-- Information acquired via protocol DNS in 159.3ms.
-- Data is authenticated: no
 也可以使用 nslookup domain 来解析，如下示例：
	
	$ nslookup baidu.com          
	Server:         127.0.0.53
	Address:        127.0.0.53#53
	Non-authoritative answer:
	Name:   baidu.com
	Address: 39.156.69.79
	Name:   baidu.com
	Address: 220.181.38.148

3、设置网卡的 DNS Server

systemd-resolve 命令可以用来设置指定网卡的 DNS Server，如下

systemd-resolve --set-dns={DNS_SERVER_IP} --interface {ITERFACE_NAME}
示例：
	
	$ sudo systemd-resolve --set-dns '8.8.8.8' --interface ens3
	$ 
	$ systemd-resolve --status | grep 'DNS Servers'
	         DNS Servers: 8.8.8.8

4、重置网卡的 DNS 设置

systemd-resolve --revert --interface {ITERFACE_NAME}
注：该命令会清除指定网上的所有 DNS Server 的设置，请慎用。

如下示例：
	
	$ sudo systemd-resolve --revert --interface=ens33

5、刷新本地 DNS 缓存

如果有时候某些域名解析出现了问题，可尝试清除本地 DNS 缓存试试。

systemd-resolve --flush-caches
示例：
	
	$ sudo systemd-resolve --flush-caches

6、查看 DNS 相关数据

systemd-resolve --statistics
示例：

	$ systemd-resolve --statistics      
	DNSSEC supported by current servers: no
	Transactions
	Current Transactions: 0
	  Total Transactions: 420
	Cache
	  Current Cache Size: 0
		  Cache Hits: 0
		Cache Misses: 0
	DNSSEC Verdicts
		      Secure: 0
		    Insecure: 0
		       Bogus: 0
	       Indeterminate: 0

注：可以看到的是清除缓存后，本地的 Cache 大小已经是 0 了，而如果我们使用 systemd-resolve domain 解析了域名，Cache 便会增加。

如下示例：

	$ systemd-resolve baidu.com          
	baidu.com: 220.181.38.148
		   39.156.69.79
	-- Information acquired via protocol DNS in 19.5ms.
	-- Data is authenticated: no
	$ 
	$ systemd-resolve taobao.com         
	taobao.com: 140.205.94.189
		    140.205.220.96
	-- Information acquired via protocol DNS in 10.1ms.
	-- Data is authenticated: no
	$ 
	$ systemd-resolve --statistics       
	DNSSEC supported by current servers: no
	Transactions
	Current Transactions: 0
	  Total Transactions: 431
	Cache
	  Current Cache Size: 2
		  Cache Hits: 0
		Cache Misses: 34
	DNSSEC Verdicts
		      Secure: 0
		    Insecure: 0
		       Bogus: 0
	       Indeterminate: 0



## nslookup 和 dig


## resolvconf
