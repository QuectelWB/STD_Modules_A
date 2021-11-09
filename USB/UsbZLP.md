USB 零包
=========

ZLP : Zero Length Packet

	#define ZLP             BIT(17) 
	...
	{ USB_DEVICE_AND_INTERFACE_INFO(QUECTEL_VENDOR_ID, QUECTEL_PRODUCT_RM500Q, 0xff, 0xff, 0x10),.driver_info = ZLP },
	...
	 option_attach：
		...
	        if (device_flags & ZLP)
                data->use_zlp = 1;
		...
	

