usb_id_table
======

## usb_device_id 
 
### option里的id_table

在 option.c里看到

	static const struct usb_device_id option_ids[] = {
		{ USB_DEVICE_INTERFACE_CLASS(0x2001, 0x7e19, 0xff),			/* D-Link DWM-221 B1 */
		  .driver_info = (kernel_ulong_t)&net_intf4_blacklist  },
		{ USB_DEVICE_INTERFACE_CLASS(0x2001, 0x7e35, 0xff),			/* D-Link DWM-222 */
		  .driver_info = (kernel_ulong_t)&net_intf4_blacklist  },
		{ USB_DEVICE_AND_INTERFACE_INFO(0x07d1, 0x3e01, 0xff, 0xff, 0xff)  }, /* D-Link DWM-152/C1 */
		{ USB_DEVICE_AND_INTERFACE_INFO(0x07d1, 0x3e02, 0xff, 0xff, 0xff)  }, /* D-Link DWM-156/C1 */
		{ USB_DEVICE_AND_INTERFACE_INFO(0x07d1, 0x7e11, 0xff, 0xff, 0xff)  }, /* D-Link DWM-156/A3 */
		{ USB_DEVICE_INTERFACE_CLASS(0x2020, 0x4000, 0xff)  },                /* OLICARD300 - MT6225 */
		{ USB_DEVICE(INOVIA_VENDOR_ID, INOVIA_SEW858)  },
		{ USB_DEVICE(VIATELECOM_VENDOR_ID, VIATELECOM_PRODUCT_CDS7)  },
		{ USB_DEVICE_AND_INTERFACE_INFO(WETELECOM_VENDOR_ID, WETELECOM_PRODUCT_WMD200, 0xff, 0xff, 0xff)  },
		{ USB_DEVICE_AND_INTERFACE_INFO(WETELECOM_VENDOR_ID, WETELECOM_PRODUCT_6802, 0xff, 0xff, 0xff)  },
		{}
		};
	MODULE_DEVICE_TABLE(usb, option_ids);

USB_DEVICE宏不管会完全匹配所有的接口；
USB_DEVICE_AND_INTERFACE_INFO的话，根据

	/**
	 * USB_DEVICE_AND_INTERFACE_INFO - describe a specific usb device with a class of usb interfaces
	 * @vend: the 16 bit USB Vendor ID
	 * @prod: the 16 bit USB Product ID
	 * @cl: bInterfaceClass value
	 * @sc: bInterfaceSubClass value
	 * @pr: bInterfaceProtocol value
	 *
	 * This macro is used to create a struct usb_device_id that matches a
	 * specific device with a specific class of interfaces.
	 *
	 * This is especially useful when explicitly matching devices that have
	 * vendor specific bDeviceClass values, but standards-compliant interfaces.
	 */
	#define USB_DEVICE_AND_INTERFACE_INFO(vend, prod, cl, sc, pr) \
		.match_flags = USB_DEVICE_ID_MATCH_INT_INFO \
			| USB_DEVICE_ID_MATCH_DEVICE, \
		.idVendor = (vend), \
		.idProduct = (prod), \
		.bInterfaceClass = (cl), \
		.bInterfaceSubClass = (sc), \
		.bInterfaceProtocol = (pr)

会根据协议类型匹配.

如果是移远的设备，也可以用，类型应该是0xff.

从源码可以发现，其实 USB_VENDOR_AND_INTERFACE_INFO 更合适。不用带上idProduct.

	#define USB_VENDOR_AND_INTERFACE_INFO(vend, cl, sc, pr) \
		.match_flags = USB_DEVICE_ID_MATCH_INT_INFO \
			| USB_DEVICE_ID_MATCH_VENDOR, \
		.idVendor = (vend), \
		.bInterfaceClass = (cl), \
		.bInterfaceSubClass = (sc), \
		.bInterfaceProtocol = (pr)

USB_DEVICE_INTERFACE_CLASS 的话

	/**
	 * USB_DEVICE_INTERFACE_CLASS - describe a usb device with a specific interface class
	 * @vend: the 16 bit USB Vendor ID
	 * @prod: the 16 bit USB Product ID
	 * @cl: bInterfaceClass value
	 *
	 * This macro is used to create a struct usb_device_id that matches a
	 * specific interface class of devices.
	 */
	#define USB_DEVICE_INTERFACE_CLASS(vend, prod, cl) \
		.match_flags = USB_DEVICE_ID_MATCH_DEVICE | \
			       USB_DEVICE_ID_MATCH_INT_CLASS, \
		.idVendor = (vend), \
		.idProduct = (prod), \
		.bInterfaceClass = (cl)

其中的blacklist结构

	struct option_blacklist_info {
		/* bitmask of interface numbers blacklisted for send_setup */
		const unsigned long sendsetup;
		/* bitmask of interface numbers that are reserved */
		const unsigned long reserved;

	};

	static const struct option_blacklist_info four_g_w14_blacklist = {
		.sendsetup = BIT(0) | BIT(1),
	};

	static const struct option_blacklist_info alcatel_x200_blacklist = {
		.sendsetup = BIT(0) | BIT(1),
		.reserved = BIT(4),
	};

因此如果是EC20设备的话，应该写

	static const struct option_blacklist_info alcatel_x200_blacklist = {
		.sendsetup = BIT(0) | BIT(1) | BIT(2) | BIT(3),
		.reserved = BIT(4),
	};

这样写就可以避免在option_probe 里判断了。

### qmi_wwan 里的id_table

用户可能会出现勾选了系统自带的qmi_wwan,但是EC20却无法匹配上qmi_wwan驱动的情况。

需要添加 patch https://lkml.org/lkml/2019/6/9/270

static const struct usb_device_id products[] = {
	{QMI_FIXED_INTF(0x03f0, 0x4e1d, 8)},	/* HP lt4111 LTE/EV-DO/HSPA+ Gobi 4G Module */
	{QMI_FIXED_INTF(0x03f0, 0x9d1d, 1)},	/* HP lt4120 Snapdragon X5 LTE */
	{QMI_FIXED_INTF(0x22de, 0x9061, 3)},	/* WeTelecom WPD-600N */
	{QMI_FIXED_INTF(0x1e0e, 0x9001, 5)},	/* SIMCom 7230E */
	{QMI_QUIRK_SET_DTR(0x1e0e, 0x9001, 5)},	/* SIMCom 7100E, 7230E, 7600E ++ */
	{QMI_QUIRK_SET_DTR(0x2c7c, 0x0125, 4)},	/* Quectel EC25, EC20 R2.0  Mini PCIe */
	{QMI_QUIRK_SET_DTR(0x2c7c, 0x0121, 4)},	/* Quectel EC21 Mini PCIe */
}

其中 QMI_FIXED_INTF 顾名思义就是 接口描述符匹配匹配。
而QMI_QUIRK_SET_DTR

	/* devices requiring "set DTR" quirk */
	#define QMI_QUIRK_SET_DTR(vend, prod, num) \
		USB_DEVICE_INTERFACE_NUMBER(vend, prod, num), \
		.driver_info = (unsigned long)&qmi_wwan_info_quirk_dtr


	enum qmi_wwan_quirks {
		QMI_WWAN_QUIRK_DTR = 1 << 0,	/* needs "set DTR" request */
	};

	static const struct driver_info	qmi_wwan_info_quirk_dtr = {
		.description	= "WWAN/QMI device",
		.flags		= FLAG_WWAN,
		.bind		= qmi_wwan_bind,
		.unbind		= qmi_wwan_unbind,
		.manage_power	= qmi_wwan_manage_power,
		.rx_fixup       = qmi_wwan_rx_fixup,
		.data           = QMI_WWAN_QUIRK_DTR,
	};
