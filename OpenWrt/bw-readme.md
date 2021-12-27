#说明

## info

MT7620

git clone https://github.com/coolsnowwolf/lede.git
cd lede
./scripts/feeds update -a
./scripts/feeds install -a

https://www.red-yellow.net/%E8%BF%85%E9%9B%B7%E4%B8%8B%E8%BD%BD%E5%AE%9D%E5%88%B7openwrt%E7%B3%BB%E7%BB%9F.html


git@github.com:esdiot/lede.git

关键是下载宝是mips 架构的。

- Target SYstem (Mediatek Ralink MIPS)
- subtaget (MT7621 based boards)
- Target Profile (Thunder Timeclound)

output:
	
	bin/targets/ramips/mt7621/openwrt-ramips-mt7621-thunder_timeclound-squashfs-sysupgrade.bin

通过Breed将自编译固件刷入下载宝

192.168.1.1




