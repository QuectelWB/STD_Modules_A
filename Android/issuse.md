
客户将IRadio 配置成1.1
无法发送请求SET_LINK_CAPACITY_REPORTING_CRITERIA
	
	05-23 10:01:11.761 D/RILJ    ( 1739): serviceDied [PHONE0]
	05-23 10:01:11.762 D/RILJ    ( 1739): handleMessage: EVENT_RADIO_PROXY_DEAD cookie = 5 mRadioProxyCookie = 5 [PHONE0]
	05-23 10:01:11.764 D/RilRequest( 1739): [3339]< SET_LINK_CAPACITY_REPORTING_CRITERIA error: com.android.internal.telephony.CommandException: RADIO_NOT_AVAILABLE ret= result=null
	05-23 10:01:11.765 D/RilRequest( 1739): [3340]< SET_LINK_CAPACITY_REPORTING_CRITERIA error: com.android.internal.telephony.CommandException: RADIO_NOT_AVAILABLE ret= result=null
	
	05-23 10:07:04.830 D/RILJ    ( 1739): [3242]> SET_LINK_CAPACITY_REPORTING_CRITERIA [PHONE0]
	05-23 10:07:04.830 E/RILJ    ( 1739): setLinkCapacityReportingCriteria ignored on IRadio version less than 1.2 [PHONE0]
	05-23 10:07:04.832 D/RILJ    ( 1739): [3243]> SET_LINK_CAPACITY_REPORTING_CRITERIA [PHONE0]
	05-23 10:07:04.832 E/RILJ    ( 1739): setLinkCapacityReportingCriteria ignored on IRadio version less than 1.2 [PHONE0]
	05-23 10:07:04.833 D/RILJ    ( 1739): [3244]> SET_LINK_CAPACITY_REPORTING_CRITERIA [PHONE0]
	05-23 10:07:04.833 E/RILJ    ( 1739): setLinkCapacityReportingCriteria ignored on IRadio version less than 1.2 [PHONE0]

	05-23 10:08:07.871 D/RILJ    ( 1739): NOTE: mWakeLockCount is 8at time of clearing
	05-23 10:08:07.875 D/RILJ    ( 1739): WAKE_LOCK_TIMEOUT  mRequestList=8
	05-23 10:08:07.875 D/RILJ    ( 1739): 0: [701353224] SET_LINK_CAPACITY_REPORTING_CRITERIA
	05-23 10:08:07.875 D/RILJ    ( 1739): 1: [701353225] SET_LINK_CAPACITY_REPORTING_CRITERIA
	05-23 10:08:07.875 D/RILJ    ( 1739): 2: [701353226] SET_LINK_CAPACITY_REPORTING_CRITERIA
	05-23 10:08:07.875 D/RILJ    ( 1739): 3: [701353227] SET_LINK_CAPACITY_REPORTING_CRITERIA
	05-23 10:08:07.875 D/RILJ    ( 1739): 4: [701353241] SET_LINK_CAPACITY_REPORTING_CRITERIA
	05-23 10:08:07.876 D/RILJ    ( 1739): 5: [701353242] SET_LINK_CAPACITY_REPORTING_CRITERIA
	05-23 10:08:07.876 D/RILJ    ( 1739): 6: [701353243] SET_LINK_CAPACITY_REPORTING_CRITERIA
	05-23 10:08:07.876 D/RILJ    ( 1739): 7: [701353244] SET_LINK_CAPACITY_REPORTING_CRITERIA

从这个日志看，这个请求至少需要IRadio 1.2

RILJ:
	
	    @Override
	    public void setSignalStrengthReportingCriteria(SignalThresholdInfo signalThresholdInfo,
		    int ran, Message result) {
		IRadio radioProxy = getRadioProxy(result);
		if (radioProxy != null) {
		    if (mRadioVersion.less(RADIO_HAL_VERSION_1_2)) {
			riljLoge("setSignalStrengthReportingCriteria ignored on IRadio version less "
				+ "than 1.2");
			return;
		    }

所以Android12 就没法配置IRadio 1.1 了


