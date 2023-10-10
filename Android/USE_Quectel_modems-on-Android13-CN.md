# Android 13 上使用移远模组

大部分内容同样也适用于Android 10-12.

## 概述

按照 Quectel_Android_RIL_Driver_User_Guide_V2.0.pdf ，
如果Android 10 上，RIL移植的时候，配置成IRadio 1.0 或者1.1，一般问题不大。 
如果Android 10 以上，尤其是Android 12、Android 13，或者要求使用5G模组，会遇到各种问题。

原生的Android是只支持到 android.hardware.radio@1.1
http://aospxref.com/android-13.0.0_r3/xref/hardware/ril/libril/Android.mk
这对Android 8 和 Android 9 影响不大。



## RIL

### kernel 驱动



### rild

### libril 和 refence-ril


### 非必须的、不建议的 chat /ip-up /ip-down

### HIDL support

#### HIDL 相关依赖的库编译

移远提供的libril.so 和 libreference-ril.so 会依赖这些库
譬如在Android 12、13 上，配置成IRadio1.5会依赖

	libhwbinder
	libhidltransport
	android.hardware.radio@1.0.so
	android.hardware.radio@1.1.so
	android.hardware.radio@1.2.so
	android.hardware.radio@1.3.so
	android.hardware.radio@1.4.so
	android.hardware.radio@1.5.so
	android.hardware.radio.config@1.0.so
	android.hardware.radio.config@1.1.so
	android.hardware.radio.config@1.2.so

这些库
后面的这几个库源码在

hardware/interfaces/radio 里

#### 添加IRadio 1.4/1.5 的兼容矩阵问题


#### Android 13 不要加IRadioConfig


### device/vendor 目录中和RIL有关的一些配置


### phone 模块


### framework 中建议修改的地方


### loopback 测试


### 





