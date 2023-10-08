# ModemManager 的问题

## 端口占用

查看modemManager 占用了哪些AT口

	root@ll:/buildroot/quec/tools/qlog# ls /proc/`pgrep ModemManager`/fd
	0  1  10  11  12  13  14  15  2  3  4  5  6  7  8  9
	root@ll:/buildroot/quec/tools/qlog# ls /proc/`pgrep ModemManager`/fd  -l
	total 0
	lr-x------ 1 root root 64 Aug 24 21:54 0 -> /dev/null
	lrwx------ 1 root root 64 Aug 24 21:54 1 -> 'socket:[30450945]'
	l-wx------ 1 root root 64 Aug 24 21:54 10 -> /run/systemd/inhibit/93.ref
	lrwx------ 1 root root 64 Aug 24 21:54 11 -> 'socket:[30453900]'
	lrwx------ 1 root root 64 Aug 24 21:54 12 -> 'socket:[30453904]'
	lrwx------ 1 root root 64 Aug 24 21:54 13 -> /dev/ttyUSB2
	lrwx------ 1 root root 64 Aug 24 21:54 14 -> /dev/ttyUSB3
	lrwx------ 1 root root 64 Aug 24 21:54 15 -> /dev/ttyUSB0
	l-wx------ 1 root root 64 Aug 24 21:54 2 -> /dev/null
	lrwx------ 1 root root 64 Aug 24 21:39 3 -> 'anon_inode:[eventfd]'
	lrwx------ 1 root root 64 Aug 24 21:39 4 -> 'anon_inode:[eventfd]'
	lrwx------ 1 root root 64 Aug 24 21:54 5 -> 'socket:[30450960]'
	lrwx------ 1 root root 64 Aug 24 21:54 6 -> 'socket:[30450976]'
	lrwx------ 1 root root 64 Aug 24 21:54 7 -> 'anon_inode:[eventfd]'
	lrwx------ 1 root root 64 Aug 24 21:54 8 -> 'socket:[30452041]'
	lrwx------ 1 root root 64 Aug 24 21:54 9 -> 'socket:[30453899]'


    
    root@ll:/buildroot/quec/tools/qlog# fuser /dev/cdc-wdm0

    /dev/cdc-wdm0:       969483
    root@ll:/buildroot/quec/tools/qlog# pgrep ModemManager

    969210

    root@ll:/buildroot/quec/tools/qlog# ps -efl | grep -i 969483

    0 S root      969483       1  0  80   0 - 61202 do_pol 21:39 ?        00:00:00 /usr/libexec/mbim-proxy
    0 S root      988564  967668  0  80   0 -  4434 pipe_r 21:58 pts/13   00:00:00 grep --color=auto -i 969483

不是ModemManager 直接打开QMI/MBIM节点的，而是这个 mbim-proxy


## 跟踪 cdc-wdm


