## 现象

服务器流量很少的时候很难出现，一般都是在业务流量跑满，单进程cpu跑满的情况下，偶尔会出现dropped数值不断增加。

## 解决方法

先给网卡更换个插槽、更换模块、更换光纤，确定内存条都正常识别的情况下，如果都没解决再往下看

通过top 1查看个别cpu软中断严重，cpu偏压严重导致软中断不均，通过修改网卡多队列模式解决，最优方式RFS,默认禁用。

/proc/sys/net/core/rps_sock_flow_entries

设置此文件至同时活跃连接数的最大预期值。对于中等服务器负载，推荐值为 32768 。所有输入的值四舍五入至最接近的2的幂

/sys/class/net/device/queues/rx-queue/rps_flow_cnt

将 device 改为想要配置的网络设备名称（例如，eth0），将 rx-queue 改为想要配置的接收队列名称（例如，rx-0）。

将此文件的值设为 rps_sock_flow_entries 除以 N，其中 N 是设备中接收队列的数量。例如，如果 rps_flow_entries 设为 32768，并且有 16 个配置接收队列，那么

rps_flow_cnt 就应设为 2048。对于单一队列的设备，rps_flow_cnt 的值和 rps_sock_flow_entries 的值是一样的

ls /sys/class/net/eth0/queues/rx-*|grep queues|wc -l

8

rps_flow_cnt=32768/8=4096

echo 32768 >/proc/sys/net/core/rps_sock_flow_entries

for rxdir in /sys/class/net/eth0/queues/rx-*

do

    echo $rps_cpus >$rxdir/rps_cpus

    echo $rps_flow_cnt >$rxdir/rps_flow_cnt

done

echo 32768 >/proc/sys/net/core/rps_sock_flow_entries


	rps_cpus='ffff,ffffffff'
	rps_flow_cnt=4096
	for ka in p4p1 p4p2
	do
	for rxdir in /sys/class/net/$ka/queues/rx-*
	do
	echo $rps_cpus >$rxdir/rps_cpus
	echo $rps_flow_cnt >$rxdir/rps_flow_cnt
	done
	done
	echo 131072 >/proc/sys/net/core/rps_sock_flow_entries



