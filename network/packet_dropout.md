#丢包问题分析

https://www.cnblogs.com/pugang/p/13609385.html

丢包问题的排查过程

可能原因


- 防火墙拦截
- 连接跟踪表溢
- Ring Buffer溢出 - 网卡驱动
- netdev_max_backlog溢出
- 反向路由过滤
- 半连接队列溢出
- /proc/sys/net/ipv4/tcp_tw_recycle 控制




