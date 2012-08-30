Ext.namespace("JsHelper");

// 加载tab页时 需要的重绘操作
JsHelper.ExtTabDoLayout = function(o) {
	var tab = Ext.getCmp("msamainframe").getActiveTab();
	tab.add(o);
	tab.doLayout();
};
