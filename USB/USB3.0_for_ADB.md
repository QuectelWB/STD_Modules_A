# USB 3.0 支持adb

修改adb server

https://askubuntu.com/questions/487663/unable-to-detect-an-adb-device-on-usb3-0	
		
	
	
	diff --git a/adb/usb_linux.c b/adb/usb_linux.c
	index 7bf2057..52e6412 100644
	--- a/adb/usb_linux.c
	+++ b/adb/usb_linux.c
	@@ -160,6 +160,7 @@ static void find_usb_device(const char *base,
		     unsigned zero_mask = 0;
		     unsigned vid, pid;
		     size_t desclength;
	+           int is_ss = 0; /* boolean */

		     if(badname(de->d_name)) continue;
		     snprintf(devname, sizeof devname, "%s/%s", busname, de->d_name);
	@@ -195,6 +196,10 @@ static void find_usb_device(const char *base,
		     vid = device->idVendor;
		     pid = device->idProduct;
		     DBGX("[ %s is V:%04x P:%04x ]\n", devname, vid, pid);
	+           if (device->bcdUSB == 0x0300) {
	+                    D("super speed device found: V:%04x P:%04x\n", vid, pid);
	+                   is_ss = 1;
	+           }

			 // should have config descriptor next
		     config = (struct usb_config_descriptor *)bufptr;
	@@ -237,6 +242,8 @@ static void find_usb_device(const char *base,
				     // looks like ADB...
				 ep1 = (struct usb_endpoint_descriptor *)bufptr;
				 bufptr += USB_DT_ENDPOINT_SIZE;
	+                       if (is_ss)
	+                               bufptr += 6; /* USB_DT_SS_EP_COMP_SIZE */
				 ep2 = (struct usb_endpoint_descriptor *)bufptr;
				 bufptr += USB_DT_ENDPOINT_SIZE;

