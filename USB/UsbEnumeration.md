#枚举遇到的问题

USB 枚举识别过程

![](usb_enumeration.png)




在Quectel的文档里只提过
	
	urb->transfer_flags |= URB_ZERO_PACKET;
	
而关于这个URB_ZERO_PACKET的描述



https://blog.csdn.net/flydream0/article/details/70215667


在进行USB CDC类开发时，无法从设备端向主机端发送64整数倍数据，最本质的原因就是，当发送数据长度恰好是Data In端点的最大包长整数倍时，最后一包数据必须是零长度的数据包(ZLP)。这是由于在USB标准中，接收端并不是通过已经接收的数据长度来判断是否接收完成，且发送端也并没有给出将要发送多长的数据，因此，接收端在接收数据前，并不知道将要接收的数据是多少，那么，问题就来了，接收端又是如何判断当前的数据已经全部接收了呢？有两点：

	若接收到的数据包长不足最大包长时，则认为当前传输完成
	如接收到的数据包长为零时，则认为当前传输完成。

由于上述两种判断，当传输的数据刚好是端点的最大包长时，当发送完最后一包(比如64个字节)时，接收端无法判断是否传输结束，进而继续等待下一包数据。这个就是问题本质所在。

对应的Solution: 

在发送完最后一包数据后，判断发送的包长是否为端点最大包长的整数倍，如是，则补发一个零长度的数据包(ZLP)。

## 识别不到


### 类型一 

反映开机过程，USB设备和主控同时开机，模块不能识别，但是等开机后，重新reset usb设备，usb设备可以被识别到。
	
	
	usb 1-1: new full speed USB device usings3c2410-ohci and address 2
	usb 1-1: device descriptor read/64, error -62
	usb 1-1: device descriptor read/64, error -62
	usb 1-1: new full speed USB device using s3c2410-ohci andaddress 3
	usb 1-1: device descriptor read/64, error -62
	usb 1-1: device descriptor read/64, error -62
	usb 1-1: new full speed USB device using s3c2410-ohci andaddress 4
	usb 1-1: device not accepting address 4, error -62
	usb 1-1: new full speed USB device using s3c2410-ohci andaddress 5
	usb 1-1: device not accepting address 5, error -62
	hub 1-0:1.0: unable to enumerate USB device on port 1
	————————————————

这种情况，根据网上的经验，主控在开机过程中，识别到了USb设备，但是主控的USB时钟没有打开导致。 https://www.cnblogs.com/dakewei/p/5927556.html

有人说 modeprobe -r ehci_hcd 卸载驱动重装...

### 类型二

https://forums.quectel.com/t/rm510q-gl-failing-to-set-usb-configuation/15690/5

这种情况
	
	can't set config #1, error -32
	
或者其他情况

	can‘t set config #1, error -12
	https://blog.csdn.net/phmatthaus/article/details/124119661


can't set config #1, error -32
	
	ffff93e291217840 3537482731 S Ci:2:002:0 s 80 06 0100 0000 0008 8 <
	ffff93e291217840 3537483792 C Ci:2:002:0 0 8 = 12012003 00000009
	ffff93e291217840 3537483798 S Co:2:002:0 s 00 31 0028 0000 0000 0
	ffff93e291217840 3537483856 C Co:2:002:0 0 0
	ffff93e291217840 3537483861 S Ci:2:002:0 s 80 06 0100 0000 0012 18 <
	ffff93e291217840 3537484053 C Ci:2:002:0 0 18 = 12012003 00000009 7c2c0008 14040102 0301
	ffff93e291217840 3537484058 S Ci:2:002:0 s 80 06 0f00 0000 0005 5 <
	ffff93e291217840 3537484233 C Ci:2:002:0 0 5 = 050f2a00 03
	ffff93e291217840 3537484237 S Ci:2:002:0 s 80 06 0f00 0000 002a 42 <
	ffff93e291217840 3537484406 C Ci:2:002:0 0 42 = 050f2a00 03071002 06000000 0a100300 0f000101 f4011410 0a000100 00000011
	ffff93e291217840 3537484413 S Ci:2:002:0 s 80 06 0200 0000 0009 9 <
	ffff93e291217840 3537484590 C Ci:2:002:0 0 9 = 09022501 050104a0 70
	ffff93e291217840 3537484593 S Ci:2:002:0 s 80 06 0200 0000 0125 293 <
	ffff93e291217840 3537484772 C Ci:2:002:0 0 293 = 09022501 050104a0 70090400 0002ffff 30000705 81020004 00063000 00000007
	ffff93e291217cc0 3537484781 S Ci:2:002:0 s 80 06 0300 0000 00ff 255 <
	ffff93e291217cc0 3537484966 C Ci:2:002:0 0 4 = 04030904
	ffff93e291217cc0 3537484969 S Ci:2:002:0 s 80 06 0302 0409 00ff 255 <
	ffff93e291217cc0 3537485143 C Ci:2:002:0 0 20 = 14035200 4d003500 31003000 51002d00 47004c00
	ffff93e291217cc0 3537485148 S Ci:2:002:0 s 80 06 0301 0409 00ff 255 <
	ffff93e291217cc0 3537485317 C Ci:2:002:0 0 16 = 10035100 75006500 63007400 65006c00
	ffff93e291217cc0 3537485320 S Ci:2:002:0 s 80 06 0303 0409 00ff 255 <
	ffff93e291217cc0 3537485490 C Ci:2:002:0 0 18 = 12033100 32003500 62003500 31003200 3200
	ffff93e299c4bf00 3537488315 S Co:2:002:0 s 00 09 0001 0000 0000 0
	ffff93e299c4bf00 3537489147 C Co:2:002:0 -32 0

最后一行的 -32 就是主控的返回的错误-32



18 = 12012003 00000009 7c2c0008 14040102 0301

这是读到设备描述符信息


