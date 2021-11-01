安卓常见问题
=====

## 关键字搜索

获取运营商信息

	onRequest: OPERATOR

拨号
	onRequest: SETUP_DATA_CALL

	requestSetPreferredNetworkType PREF_NET_TYPE_LTE_GSM_WCDMA

	DcActivatingState: msg={ what=EVENT_SETUP_DATA_CONNECTION_DONE
		
	
	onSetupConnectionCompleted received DataCallResponse: DataCallResponse: {version=11 status=0 retry=-1 cid=1 active=1 type=IP ifname=ppp0 mtu=1500 addresses=[10.192.223.115/32] dnses=[100.100.2.136,100.100.2.138] gateways=[0.0.0.0] pcscf=[]}

### SELINUX

	android, SELinux, avc: denied, audit


## 常见问题1

没有设备节点

和selinux有关。

	10-21 15:23:23.808 D/RIL RILU( 1773): find /sys/bus/usb/devices/1-1:1.4/usbmisc/cdc-wdm0
	10-21 15:23:23.808 D/RIL RILU( 1773): qmichannel = cdc-wdm0
	10-21 15:23:23.809 E/RIL RILU( 1773): Cannot access file:/dev/cdc-wdm0, errno = 2(No such file or directory)
	10-21 15:23:23.809 E/RIL RILU( 1773): patch for 'system/core/init/devices.cpp' is required!
	10-21 15:23:23.809 E/RIL RILU( 1773): ***********************************************************************************************
	10-21 15:23:23.809 E/RIL RILU( 1773): diff --git a/system/core/init/devices.cpp b/system/core/init/devices.cpp
	10-21 15:23:23.809 E/RIL RILU( 1773): index ada1e28..812c60d 100755
	10-21 15:23:23.809 E/RIL RILU( 1773): --- a/system/core/init/devices.cpp
	10-21 15:23:23.809 E/RIL RILU( 1773): +++ b/system/core/init/devices.cpp
	10-21 15:23:23.809 E/RIL RILU( 1773): @@ -407,6 +407,10 @@ void DeviceHandler::HandleDeviceEvent(const Uevent& uevent) {
	10-21 15:23:23.809 E/RIL RILU( 1773):          int device_id = uevent.minor % 128 + 1;
	10-21 15:23:23.809 E/RIL RILU( 1773):          devpath = StringPrintf("/dev/bus/usb/%03d/%03d", bus_id, device_id);
	10-21 15:23:23.809 E/RIL RILU( 1773):          }
	10-21 15:23:23.809 E/RIL RILU( 1773): +#if 1 //add by quectel for mknod /dev/cdc-wdm0
	10-21 15:23:23.809 E/RIL RILU( 1773): +       } else if (uevent.subsystem == "usbmisc" && !uevent.device_name.empty()) {
	10-21 15:23:23.809 E/RIL RILU( 1773): +        devpath = "/dev/" + uevent.device_name;
	10-21 15:23:23.809 E/RIL RILU( 1773): +#endif
	10-21 15:23:23.809 E/RIL RILU( 1773):      } else if (StartsWith(uevent.subsystem, "usb")) {
	10-21 15:23:23.809 E/RIL RILU( 1773):          // ignore other USB events
	10-21 15:23:23.809 E/RIL RILU( 1773):          return;


## 常见问题2 

- 拿到ip地址了，不能上网.

	更新qmi_wwan


## 常见问题3 

- Q:安卓11 图标不对。

	更新ril库。

## 

