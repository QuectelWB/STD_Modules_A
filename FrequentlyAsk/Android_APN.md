#APN

https://blog.csdn.net/sjz4860402/article/details/78522871


APN的完整说明在3GPP规范TS23.003 Clause 9中进行了详细定义。
MCCMNC的定义在3GPP规范TS23.003 Clause 2的IMSI定义中。

APN在GPRS骨干网中用来标识要使用的外部PDN（Packet data network，分组数据网，即常说的Internet），在GPRS网络中代表外部数据网络的总称。

APN由以下两部分组成：

- APN网络标识：

是用户通过GGSN/PGW（Gateway GPRS Support Node，GPRS网关支持节点/PDN Gateway ，分组数据网网关）可连接到外部网络的标识，该标识由网络运营者分配给ISP（Internet Service Provider，因特网业务提供者）或公司，与其固定Internet域名一致，是APN的必选组成部分。例如 ， 定义移动用户通过该接入某公司的企业网，则APN的网络标识可以规划为“www.ABC123.com”。

- APN运营者标识：

用于标识GGSN/PGW所归属的网络，是APN的可选组成部分。其形式为“MNCxxxx.MCCyyyy.gprs”（3G网络中），或者“MNCxxxx.MCCyyyy.3gppnetwork.org（4G网络中）。APN实际上就是对一个外部PDN的标识，这些PDN包括企业内部网、Internet、WAP网站、行业内部网等专用网络。


Apn在手机中的存储：

apn文件：

	System/etc/apn-conf.xml

apn数据存储的数据库：
	
	/data/data/com.android.providers.telephony/databases/ telephony.db

Carriers表

apn的初始化：

在启动手机时，需要初始化telephony.db数据库，这时候会读取手机目录System/etc/apn-conf.xml并把其中的内容加入到Carriers表中。以后查询有关apn的配置参数都是从Carriers表中取出。

创建并初始化Carriers表：
	
	packages/providers/TelephonyProvider/src/com/android/providers/telephony/TelephonyProvider.java

内部类：DatabaseHelper.java

//创建数据库

由于添加了UNIQUE 约束条件，如果两个差不多的apn参数满足约束条件内的属性都相等，那么认为是同一组apn参数，将不会重复插入到数据库。






