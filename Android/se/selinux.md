
## 确定 SELinux 问题
​ 在调试过程中遇到权限问题时，可以通过如下方法，确定是不是由于 SELinux 导致的问题：

方法一：
通过串口或者adb使用如下命令，先将 selinux权限切换到审查模式：

setenforce 0 （临时关闭 SELinux 的限制访问模式）
getenforce  （得到结果为Permissive)
方法二：
​ 一般情况下，user 版本的固件才会打开 SELinux 权限，会发生一些权限问题，此时可以用 debug 版本的固件，来进行测试，是否还会有上述的问题发生。因为 debug 版本的固件一般不会打开 SELinux，即此时的 SELinux是审查模式，只会出现警告，但是不会报错。（注：此方法要确定，debug版本的固件是否会打开 SELinux权限，如果debug版本也开启了 SELinux 权限，则这个方法不适用。）

方法三：
​ 如果看到如下的log信息，一定可以确定是确定某些 SELinux 的权限：

avc: denied { read write } for pid=231 comm="eGTouchD" name="ttyS1" dev="tmpfs" ino=257 scontext=u:r:eGTouchD:s0 tcontext=u:object_r:serial_device:s0 tclass=chr_file permissive=0
avc: denied { read write } for pid=231 comm="eGTouchD" name="ttyS2" dev="tmpfs" ino=292 scontext=u:r:eGTouchD:s0 tcontext=u:object_r:serial_device:s0 tclass=chr_file permissive=0

使用上述三种方法，可以确定是否是 SELinux 造成的问题。如果确定是 SELinux 权限造成的问题，可以通过下面的方法来解决SELinux 权限问题。

## 处理 SELinux 权限问题
 通过上面的方法已经确定是 SELinux 造成的问题了，那么接下来通过如下方法，来解决 SELinux 权限问题。

### 从BoardConfig.mk中修改
文件路径 device/rockchip/common/BoardConfig.mk

BOARD_SELINUX_ENFORCING ?= false

### 从selinux.cpp中强制关闭SELINUX，代码如下，
目录：
~/rk3399_android10/system/core$
修改：
diff --git a/init/selinux.cpp b/init/selinux.cpp
index 86238b430..8532dfa3c 100644
--- a/init/selinux.cpp
+++ b/init/selinux.cpp
@@ -97,6 +97,7 @@ EnforcingStatus StatusFromCmdline() {
 }
 
 bool IsEnforcing() {
+    return false;
     if (ALLOW_PERMISSIVE_SELINUX) {
         return StatusFromCmdline() == SELINUX_ENFORCING;
     }

### 修改文件：system\core\init\init.cpp, 添加代码（is_enforcing = 0;）
 bool kernel_enforcing = (security_getenforce() == 1);
        bool is_enforcing = selinux_is_enforcing();
        is_enforcing = 0;
        if (kernel_enforcing != is_enforcing) {

### 修改文件：kernel\arch\arm64\boot\dts\rockchip\px30-android.dtsi，
添加代码androidboot.selinux=permissive
--- a/arch/arm64/boot/dts/rockchip/px30-android.dtsi
+++ b/arch/arm64/boot/dts/rockchip/px30-android.dtsi
@@ -6,7 +6,7 @@
 
 / {
        chosen: chosen {
-               bootargs = "earlyprintk=uart8250,mmio32,0xff160000 swiotlb=1 console=ttyFIQ0 androidboot.baseband=N/A androidboot.veritymode=enforcing androidboot.hardware=rk30board androidboot.console=ttyFIQ0 init=/init kpti=0";
+               bootargs = "earlyprintk=uart8250,mmio32,0xff160000 swiotlb=1 console=ttyFIQ0 androidboot.baseband=N/A androidboot.selinux=permissive androidboot.veritymode=enforcing androidboot.hardware=rk30board androidboot.console=ttyFIQ0 init=/init kpti=0";
        };
 
        fiq-debugger {

### 从kernel中彻底关闭
修改LINUX/android/kernel/arch/arm64/configs/xxx_defconfig文件（xxx一般为手机产品名）， 去掉CONFIG_SECURITY_SELINUX=y 的配置项

### sepolicy中添加权限
首使用adb命令，从众多log信息中拾取与SELinux 权限相关的log，命令如下：

adb shell dmesg | grep avc > avc.txt #ubuntu使用这个命令
 
adb shell dmesg | findstr avc > avc.txt #win使用这个命令
通过上述命令，我们已经得到了一个只包含 SELinux 权限的所以log信息，列举一条log进行解析。

avc:  denied  { set } for property=ro.ril.ecclist pid=1 uid=0 gid=0 scontext=u:r:vendor_init:s0 tcontext=u:object_r:radio_prop:s0 tclass=property_service permissive=0
方法一(纯手撸)：
​ 根据log的固定格式，进行手动的一一解析，log格式如下：

avc: denied { 操作权限 } for pid=7201 comm=“进程名” scontext=u:r:源类型:s0 tcontext=u:r:目标类型:s0 tclass=访问类型 permissive=0
 
#============= 源类型 ==============
allow 源类型 目标类型:访问类型 操作权限;
完整的意思是： "源类型"进程对"目标类型"的"访问类型"缺少 "操作权限"
​ 根据上边的格式，手动撸出来应该是这样的：

#============= vendor_init ==============
allow vendor_init radio_prop:property_service set;
方法二(用工具解析，快的一批)：
​ 因为 SELinux 造成的权限问题会非常多，如果手动一条一条的解析，那可能会被恶心死，所以我们可以使用如下工具，直接解析生成的 avc.txt文本，会将 avc.txt 文本中所有的权限问题，全部解析出来，速度非常之快。

举个例子：

​ 在实际的开发项目中，遇到了 SELinux 权限问题，使用如下方法解析系统报出的 avc 权限问题。

1.adb shell dmesg | grep avc > avc.txt        #将kernel 中报出的所有权限问题，提取到avc.txt文件
2.audit2allow -i avc.txt            #使用 audit2allow 工具解析avc.txt
3.解析出来如下图所示：
android$ audit2allow -i avc.txt
    #============= eGTouchD ==============                //对应的是 eGTouchD.te 文件。
    allow eGTouchD device:chr_file { read write };
    allow eGTouchD self:capability dac_override;
    allow eGTouchD serial_device:chr_file { read write };
    allow eGTouchD shell_exec:file execute;
4.上述步骤解析完了报出的权限问题，接下来需要把，这些权限添加到对应的 eGTouchD.te 文件中，直接将上述的四条 allow xxx 拷贝到 eGTouchD.te即可。
5.加入上述权限后，重新编译固件，如果编译时遇到一些类似这样的报错：neverallow xxxx， 根据提示去关闭相应的nerverallow 即可。
​ 安装 audit2allow 工具：

sudo apt-get install policycoreutils       # 安装工具
sudo apt-get install policycoreutils-python-utils
sudo apt-get autoremove                    # 安装有问题时执行

​ 如果使用 audit2allow 工具时有如下报错的话，可以通过下面的方法解决：

遇到如下错误时：
    dxq@dxq-x:~/log$ 
    dxq@dxq-x:~/log$ audit2allow -i log.txt 
    ValueError: You must specify the -p option with the path to the policy file.
    The above exception was the direct cause of the following exception:
 
    Traceback (most recent call last):
      File "/usr/bin/audit2allow", line 370, in <module>
        app.main()
      File "/usr/bin/audit2allow", line 354, in main
        audit2why.init()
    SystemError: <built-in function init> returned a result with an error set
    dxq@dxq-x:~/log$ 
 
解决办法：
    sudo vim  /usr/bin/audit2allow
     注释掉如下4行代码，这个错误是因为执行audit2why.init()导致的，具体原因不详。
     def main(self):
            try:
                self.__parse_options()
                # if self.__options.policy:
                #     audit2why.init(self.__options.policy)
                # else:
                #     audit2why.init()

##  Tips小技巧

以上 .te 文件的路径一般在如下两个位置：
#### Google原生目录
system/sepolicy
#### 厂家目录，以rk为例
device/rockchip/common/sepolicy

编译时，厂商的 .te 文件会覆盖 Google 中原生的.te 文件，一般情况下，能在厂商目录下修改，就在厂商目录下修改。因为修改 Google 目录下的 .te文件，可能会引起 CTS fail 问题，但是修改厂商目录下的 .te文件，则不会影响 CTS。修改完后，只需要单独编译 boot.img 固件就行。

绝大多数的情况下 avc denied的报错不会一次性的全部暴露出来，要解决完一个，才会报下一个问题。
比如：一个进程需要读、写和打开的权限，但是一般情况下，可能会先报其中一个权限缺失，等你加了这个权限后，才会报另一个权限缺失，以此类推。

此时可以通过下面两种方法处理：

方法一：
如果固件是debug版本，一般不会打开 SELinux，所以在debug版本中，只会报 avc denied的警告，但是不会报错，可以在debug 版本上，把要运行的程序都运行一次，然后把所有 avc denied 相关的log 抓取来，一次性加完。

方法二：
针对dir缺少的任何权限，建议赋予create_dir_perms，基本涵盖对dir的所有权限，比如：
{ open search write read rename create rmdir getattr }等等。
针对file缺少的任何权限，建议赋予rwx_file_perms，基本涵盖对file的所有权限，比如：
包含{ open read write open execute getattr create ioctl }等等。


## selinux 单独验证

编译：（只针对 .te 的文件， 如果file_contexts service_contexts 这种文件好像不行）

make selinux_policy -j8  // vendor 和system 都修改了

make  selinux_policy_nonsystem  -j8  // 只修改 vendor相关的sepolicy

编译成功后，只需要替换如下文件 vendor_sepolicy.cil，即可快速验证

/vendor/etc/selinux/vendor_sepolicy.cil



