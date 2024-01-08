#!/bin/bash

# https://www.cnblogs.com/dashuperfect/articles/15185056.html 

function set_up()
{
 ip netns add ns1
 ip netns add ns2
 ip netns add ns3
 ip link add veth12 type veth peer name veth21
 ip link add veth23 type veth peer name veth32
 ip link set veth12 netns ns1
 ip link set veth21 netns ns2
 ip link set veth23 netns ns2
 ip link set veth32 netns ns3
 ip netns exec ns1 ip addr add 192.168.1.101/24 dev veth12
 ip netns exec ns1 ip link set veth12 up
 ip netns exec ns2 ip addr add 192.168.1.1/24 dev veth21
 ip netns exec ns2 ip link set veth21 up
 ip netns exec ns2 ip addr add 110.0.0.101/24 dev veth23
 ip netns exec ns2 ip link set veth23 up
 ip netns exec ns3 ip addr add 110.0.0.102/24 dev veth32
 ip netns exec ns3 ip link set veth32 up
 echo 1 > /proc/sys/net/ipv4/ip_forward
 ip netns exec ns2 iptables -t nat -A POSTROUTING -s 192.168.1.0/24 -o veth23 -j MASQUERADE

}

#
#
#  ip netns exec ifconfig usb0
#  ip netns exec  ns2 ifconfig usb0
#  ip netns exec  ns2 busybox udhcpc -i  usb0
#  ip netns exec  ns2 ifconfig usb0
#  ip netns exec  ns2 ping www.baidu.com
#  ip netns exec  ns2 ifconfig
# ip netns exec  ns2 route -n
# ip netns exec  ns2 iptables -t nat -A POSTROUTING -o usb0  -j MASQUERADE --random
#

function tear_down()
{
    ip netns del ns1
    ip netns del ns2
    ip netns del ns3
 }


if [ "$1" == "set_up" ];then
  set_up
elif [ "$1" == "tear_down" ];then
  tear_down
else
  echo "Nothing to do ..."
fi

