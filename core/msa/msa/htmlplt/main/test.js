function Main_Index() {

	var btn = new Ext.Button({
				text : '2323432432455565645654654',
				handler : function() {
					Ext.MessageBox.alert('1111', '2222222');
				}
			});
	var tabmainpanel = new Ext.Panel({
		    id: 'msamainpanel',
				items : [btn]
			});
	JsHelper.ExtTabDoLayout(tabmainpanel);// 注意这里把panel组件加到当前的tabpanel里
}
Main_Index();// 执行方法
