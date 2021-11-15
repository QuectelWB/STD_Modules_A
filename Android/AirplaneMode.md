


在某些开发场景中可能需要实现控制飞行模式的方法，这里列举几个控制飞行模式开关的思路。

1、通过adb命令

adb命令的方式是通过调用Settings的接口，可以在命令行窗口下使用adb命令adb shell settings put global airplane_mode_on 1开启飞行模式，通过adb shell settings put global airplane_mode_on 0关闭飞行模式。在应用中可以通过Runtime类模拟adb shell的运行环境，使用adb shell的命令运行相应的功能。

具体实现代码如下：

	private void setAirPlaneMode(boolean enable) {
	
		int mode = enable ? 1 : 0;

		String cmd = "settings put global airplane_mode_on " + mode;
		
		try {
			Runtime.getRuntime().exec(cmd);
		} 
		catch (IOException e) {
			e.printStackTrace();
		}
	}



开启

        adb shell settings put global airplane_mode_on 1

        adb shell am broadcast -a android.intent.action.AIRPLANE_MODE --ez state true

关闭

        adb shell settings put global airplane_mode_on 0

        adb shell am broadcast -a android.intent.action.AIRPLANE_MODE --ez state false






## 广播

这种方式是通过发生系统广播，通过广播处理控制飞行模式。在Android4.2即API Level > 17以后的版本，Settings.System的属性迁移到了Settings.Global下，所以如果只调用Settings.System下的AIRPLANE_MODE_ON并不能控制飞行模式的开启与关闭。

具体实现代码如下：

	private void setAirPlaneMode(Context context, boolean enable) {
		
		if (Build.VERSION.SDK_INT <= Build.VERSION_CODES.JELLY_BEAN) {
		
			Settings.System.putInt(context.getContentResolver(), Settings.System.AIRPLANE_MODE_ON, enable ? 1 : 0);

		} else {
		
			Settings.Global.putInt(context.getContentResolver(), Settings.Global.AIRPLANE_MODE_ON, enable ? 1 : 0);

		}

		Intent intent = new Intent(Intent.ACTION_AIRPLANE_MODE_CHANGED);

		intent.putExtra("state", enable);

		context.sendBroadcast(intent);

	}


