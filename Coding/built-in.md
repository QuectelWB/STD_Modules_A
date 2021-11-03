y和m的区别
=====

y: 模块驱动编译到内核中，启动时自动加载

m：模块会被编译，但是不会被编译到内核中，只是生成.o文件，我们可以收集这些.o文件做到linux的文件系统中，然后用insmod实现动态加载。



模块加载的两种方式

## 动态加载

	动态加载是将驱动模块加载到内核中，而不能放入/lib/modules/下
	在2.4内核中，加载驱动命令为：insmod ,删除模块为：rmmod；
	在2.6以上内核中，除了insmod和rmmod外，加载命令更有modprobe；
	insmod和modprobe不同之处：
	insmod 绝对路径/××.o，而modprobe ××即可，不用加.ko或.o后缀，也不用加路径；最重要的一点是：modprobe同时会加载当前模块所依赖的其他模块；
	lsmod查看当前加载到内核中的所有驱动模块，同时提供其他一些信息，比如其他模块是否在使用另一个模块。


## 静态加载

     linux设备一般分为：字符设备、块设备和网络设备，每种设备在内核原始码目录树drivers/下都有对应的目录，其加载方法类似，以下以字符设备静态加载为例，假设驱动程式原始码名为ledc.c，具体操作步骤如下：
   
- 第一步：将ledc.c源程式放入内核源码drivers/char/下；
- 第二步：修改drivers/char/Config.in文件，具体修改如下：
	
<br>  
          按照打开文件中的格式添加即可；
            在文件的适当位置（这个位置随便都能，但这个位置决定其在make menuconfig窗口中所在位置）加入以下任一段代码：
            
		    tristate ’LedDriver’ CONFIG_LEDC
		    if [ "$CONFIG_LEDC" = "y" ];then
		    bool ’  Support for led on h9200 board’ CONFIG_LEDC_CONSOLE

		    fi
	   
 说明:以上代码使用tristate来定义一个宏,表示此驱动能直接编译至内核（用*选择）,也能编制至/lib/modules/下（用M选择）,或不编译（不选）。

		    bool ’LedDriver’ CONFIG_LEDC
		    if [ "$CONFIG_LEDC" = "y" ];then
		    bool ’  Support for led on h9200 board’ CONFIG_LEDC_CONSOLE
		    fi

            说明:以上代码使用bool来定义一个宏,表示此驱动只能直接编译至内核（用*选择）或不编译（不选），不能编制至/lib/modules/下（用M选择）。
     
     第三步：修改drivers/char/Makefile文件
            在适当位置加入下面一行代码：

             obj-$(CONFIG_LEDC)  +=  ledc.o
            或在obj-y一行中加入ledc.o,如：
            obj-y += ledc.o mem.o 后面不变；

经过以上的设置就能在执行make menuconfig命令，在窗口中的character devices---> 中进行选择设置，选择后重新编译就ok了
