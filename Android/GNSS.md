Android GNSS 服务

Introduction



What is GPS?
GPS is the colloquial term used to refer to satellite navigation. Satellite navigation refers to location determination using time signals transmitted along lines of sight from satellites. The correct technical term for such navigation systems is GNSS: Global Navigation Satellite Systems.



A GPS chip is a piece of silicon that, with the help of a front-end antenna, acts as a receiver for the satellite’s navigation data.  A GPS chip typically obtains a ‘lock’ when at least 3 satellites are in its line of sight. Once the lock is obtained, the GPS chip can calculate the location and transmit this data to the host (your computer or a SoC-running Android) using a number of protocols. Common GPS data formats are NMEA (National Marine Electronics Association Format), RINEX (Receiver
Independent Exchange Format), or proprietary to the GPS chip manufacturer.  Most GPS chips use the NMEA format.

GPS 至少要介绍3颗卫星以上才会锁定位置。
GNSS位置信息一般都是输出NMEA格式的。


The GPS chips interface with the host via a serial data protocol, typically UART or I2C. In this article, we focus on the UART interface. Most steps of integration remain the same irrespective of the interface type.

GNSS 芯片一般都是I2C或者UART接口的。

Bringing up the GPS chip on the Android platform consists of two steps:

    Enabling the hardware interface between GPS chip and host
    Integrating the HAL layer for the GPS chip to Android.


## Enabling the hardware interface between GPS chip and host

    
   

1. 内核需要能输出 
Enable the required kernel driver
2. 供电、时钟正常
Ensure power and clock to the GPS chip
3. 串口配置
Serial port configuration

Integrating the HAL layer to Android Build
Once the GPS chip is powered on, it will transmit messages in the chosen protocol format at a regular interval. It is the responsibility of the user space code to interpret these messages and translate them into location information (latitude and longitude). In Android, the hardware abstraction layer (HAL) is the user space code that interprets the GPS protocol and translates it into usable information for Android Location Service.

More on Android HAL and Location Manager
Android gives your applications access to the location services supported by the device through classes in the android.location package. The central component of the location framework is the LocationManager system service, which provides APIs to determine the location and bearing of the underlying device (if available).

The LocationManager relies on android.hardware.gnss* service to provide the location information, which it can then pass on to the application.

This service is the HAL or hardware abstraction layer. The Location Manager checks for a service by this name to ascertain that GNSS is present.

HAL defines a standard interface for hardware vendors to implement, which enables Android to be agnostic about lower-level driver implementations. Using a HAL allows you to implement functionality without affecting or modifying the higher-level system.

In Android 8.0 and higher, the lower-level layers are re-written to adopt a new, more modular architecture. Devices running Android 8.0 and higher must support HALs written in the HIDL language, with few exceptions.

The GNSS HAL code is expected to provide implementations for android.hardware.gnss interfaces such as IAGnss (Assisted GNSS callbacks), IGnssGeofencing, IGnssMeasurement etc.

Integrating HAL code
The HAL code for the GPS module is provided by the GPS chip vendor.

Place the HAL code under the hardware/ folder.

Run make from the Android source code folder. Make sure the required targets, like the android.hardware.gnss@1.0-service-<vendor>.rc service and android.hardware.gnss@1.0-impl-<vendor>.so library, are built based on the proper Android OS. version.Service is placed in /system/vendor/bin/hw and .so lib is placed in /system/vendor/lib64/hw.

Make sure your HAL is being built along with the Android.

1) HAL code does not get built as part of the Android image.

Make sure HAL code is placed directly under the hardware/ folder. If your service is not being built, then run mm command in your HAL directory to build its modules and then run the Android source make.

2) GNSS service dies due to permission errors on /dev/<serial_dev>

The HAL code will come with a *.rc file, which will have the user and group under which the GNSS service will run. If the user and group on /dev/<serial_dev> differ from the one under which GNSS service is running, the services will be denied permission to open the serial device to talk with the GPS chip. Make sure proper permissions are given to the device to send and receive data. If permissions are not given, then change the user and group accordingly in the following file
ueventd.<vendor>.rc, present in /device/<vendor>/common/rootdir/etc folder.
3) SEPolicy errors:
Initial integration can be done with selinux mode set to permissive. In the permissive mode, sepolicy denials are logged but not enforced. sepolicy denials appear under dmesg as logs of the format “avc: denied”. Run these logs through audit2allow tool to generate sepolicy strings. Most vendors will provide the sepolicy .te files required for the GPS service.

If .te files are not provided by the vendor, then we can generate using this command.

adb logcat -b all -d | audit2allow -p <policy_directory_path>/<name of te file> Append file_contexts of the gps sepolicy to linux file_contexts file in /system/sepolicy/vendor and copy the .te file generated to /system/sepolicy/vendor/ folder.

4) Remove any other GNSS service from the manifest files:

Android Location Manager by default will be waiting for data provided by any one of the location services. If there are any default services being run, then disable that service and enable the service required by your module.
The manifest files are present under /device/<vendor>/<vendor_product>/ folder as manifest*.xml
Testing of the integrated GNSS module
 After the process of Integration, the next step is to test the module. The following steps help in testing the given GPS module.

 The first step is to install the appropriate Android Apk eg:Google Maps, Spyzie in a board, which can analyse the location data and display. Some vendors provide their own Android apk to test the chip.
 The ADB tool helps users to check whether the required processes and services are being run or not.
 Logcat can be used to analyse failures or crashes in the GNSS service.



