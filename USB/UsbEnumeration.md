#枚举遇到的问题

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


