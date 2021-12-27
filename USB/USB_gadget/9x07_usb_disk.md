#USB Disk

gadget上的U盘可以作为主控上的Mass Storage 设备

	diff --git a/ql-ol-rootfs/sbin/usb/compositions/9025- b/ql-ol-rootfs/sbin/usb/compositions/9025
	old mode 100644
	new mode 100755
	index ea2ea6f..3ad25b2
	--- a/ql-ol-rootfs/sbin/usb/compositions/9025
	+++ b/ql-ol-rootfs/sbin/usb/compositions/9025
	@@ -242,12 +242,14 @@ run_9607() {
		fi
		echo QTI,BAM_DMUX > /sys/class/android_usb/android0/f_rmnet/transports
		echo BAM_DMUX > /sys/class/android_usb/android0/f_usb_mbim/mbim_transports
	-	echo $USB_FUNC,ffs > /sys/class/android_usb/android$num/functions
	+	echo $USB_FUNC,ffs,mass_storage > /sys/class/android_usb/android$num/functions
		echo 1 > /sys/class/android_usb/android$num/remote_wakeup
		echo 1 > /sys/class/android_usb/android$num/f_rndis/wceis
		sleep $delay
		#echo 1 > /sys/class/android_usb/android$num/enable
	 
	+    echo /dev/mmcblk0p1 >  /sys/class/android_usb/android$num/f_mass_storage/lun/file
	+
		if [ -f /sbin/usb/compositions/quec_manufcture ]
		then 
		echo $QUEC_MANUFCTURE > /sys/class/android_usb/android$num/iManufacturer


