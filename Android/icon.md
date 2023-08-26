# Android StatusBar 加载

https://www.jianshu.com/p/8c28a8949b85

- [PhoneStatusBarPolicy：定义了系统通知图标的设置策略]( )

- [StatusBarSignalPolicy：定义了状态栏网络信号策略]( )

PhoneStatusBarPolicy 在 StatusBar 的start 方法里初始化
	
	StatusBar.java 
	@Override
	public void start() {
	    
	    // 省略部分代码......
	    mStatusBarSignalPolicy.init();

	    // 创建整个SystemUI视图并添加到WindowManager中
	    createAndAddWindows();//这个重点方法，创建相关的视图

	    // 省略部分代码......

	    // 调用图标策略来显示更新所有图标。
	    mIconPolicy.init();

	    // 省略部分代码......
	}


遇到一个问题，客户的系统上没有4G/5G图标

检查log:



	08-23 13:52:12.832  1004  1004 V ShadeControllerImpl: NotificationShadeWindow: com.android.systemui.statusbar.phone.NotificationShadeWindowView{40c1c03 I.E...... ......ID 0,0-720,1280} canPanelBeCollapsed(): false
	08-23 13:52:14.083  1004  1004 E CollapsedStatusBarFragment: biaowang onViewCreated DarkIconManager start
	08-23 13:52:29.910  1004  1004 E StatusBarSignalPolicy: Unexpected subscription 1
	08-23 13:54:25.827  1004  1004 W View    : requestLayout() improperly called by com.android.systemui.statusbar.phone.StatusIconContainer{4f30580 V.E...... ......ID 0,0-101,96 #7f0b05df app:id/statusIcons} during layout: running second layout pass
	08-23 13:54:26.034  1004  1004 W View    : requestLayout() improperly called by StatusBarIconView(slot=mobile icon=null notification=null) during second layout pass: posting in next frame
	08-23 14:01:18.051  1004  1004 W View    : requestLayout() improperly called by com.android.systemui.statusbar.phone.StatusIconContainer{4f30580 V.E...... ......ID 0,0-101,96 #7f0b05df app:id/statusIcons} during layout: running second layout pass
	08-23 14:01:18.216  1004  1004 W View    : requestLayout() improperly called by StatusBarIconView(slot=mobile icon=null notification=null) during second layout pass: posting in next frame


最终显示的是LTE 而不是4G. 和之前的结果不一样。
是因为，我把代码里的Overlay 都注释掉了





