

  mmma -j6 external/strace


  adb shell strace -f -p PID

-f 标志告诉 strace 应附加到相应进程中的所有现有线程，以及之后会产生的所有新线程。

  adb shell setprop wrap.com.android.calendar '"logwrapper strace -f -o /data/local/tmp/strace/strace.com.android.calendar.txt"'


## 在 zygote 上使用

如需在 zygote 上使用 strace，请修复相关的 init.rc zygote 行（需要使用 adb shell setenforce 0）

  cd system/core/
  patch -p1 <<EOF
  --- a/rootdir/init.zygote32.rc
  +++ b/rootdir/init.zygote32.rc
  @@ -1,4 +1,4 @@
  -service zygote /system/bin/app_process -Xzygote /system/bin --zygote --start-system-server
  +service zygote /system/bin/strace -o /data/local/tmp/zygote.strace /system/bin/app_process -Xzygote /system/bin --zygote --start-system-server
       class main
       socket zygote stream 660 root system
       onrestart write /sys/android_power/request_state wake
  EOF

## Android 启动期间的 strace 日志


  由于进程名称已从 zygote 改为 strace，给定服务可能会因缺少用于 strace 的 SELinux file_context 而无法启动。解决方法：在 system/sepolicy/private/file_contexts 中为 strace 添加一个新行，并将原始文件上下文复制过来。示例：


    /dev/socket/zygote      u:object_r:zygote_socket:s0
  + /system/bin/strace u:object_r:zygote_socket:s0


添加内核参数或 bootconfig 参数，然后在 SELinux 宽容模式下启动相应设备。为此，您可以将 androidboot.selinux=permissive 添加到 BOARD_KERNEL_CMDLINE，也可以添加到 BOARD_BOOTCONFIG（在内核版本为 5.10 或更高版本的 Android 12 中）。（此变量在 build/core/Makefile 中会变为只读，但在 /device/*/BoardConfig 下则始终可用。）



