Ext.onReady(function() {

	 
	var panel = new Ext.Panel({
		el : 'hello-tabs',
		autoTabs : true,
		deferredRender : false,
		border : false,
		baseCls : 'x-plain',
		items : {
			xtype : 'tabpanel',
			id : 'loginTabs',
			activeTab : 0,
			height : 200,
			border : false,
			items : [{
				title : "身份认证",
				xtype : 'form',
				id : 'loginForm',
				defaults : {
					width : 260
				},
        baseCls : 'x-plain',
				bodyStyle : 'padding:20 0 0 50',
				defaultType : 'textfield',
				labelWidth : 40,
				labelSeparator : '：',
				items : [{
							fieldLabel : '帐&nbsp;号',
							name : 'account',
							id : 'account',
							cls : 'user',
							blankText : '帐号不能为空,请输入!',
							maxLength : 30,
							maxLengthText : '账号的最大长度为30个字符',
							allowBlank : false,
							listeners : {
								specialkey : function(field, e) {
									if (e.getKey() == Ext.EventObject.ENTER) {
										Ext.getCmp('password').focus();
									}
								}
							}
						}, {
							fieldLabel : '密&nbsp;码',
							name : 'password',
							id : 'password',
							cls : 'key',
							inputType : 'password',
							blankText : '密码不能为空,请输入!',
							maxLength : 20,
							maxLengthText : '密码的最大长度为20个字符',
							allowBlank : false,
							listeners : {
								specialkey : function(field, e) {
									if (e.getKey() == Ext.EventObject.ENTER) {
										login();
									}
								}
							}
						}]
			},{
				title : '关于',
				contentEl : 'aboutDiv',
				defaults : {
					width : 230
				}
			}]
		}
	});
  

  var menu = new Ext.menu.Menu({
        id: 'mainMenu',
        items: [
                        // stick any markup in a menu
                        '<b class="menu-title">选择一个主题</b>',
                        {
                            text: '经典风格',
                            checked: true,
                            group: 'theme',
                            checkHandler: function() {
															setTheme(0);
														}
                        }, {
                            text: '深蓝风格',
                            checked: false,
                            group: 'theme',
                            checkHandler: function() {
															setTheme(1);
														}
                        }, {
                            text: '浅灰风格',
                            checked: false,
                            group: 'theme',
                            checkHandler: function() {
															setTheme(2);
														}
                        }, {
                            text: '金黄风格',
                            checked: false,
                            group: 'theme',
                            checkHandler: function() {
															setTheme(3);
														}
                        }
        ]
    });

	var win = new Ext.Window({
		title : 'MSA&reg - 宝创MSA互联网管理网关',
		renderTo : Ext.getBody(),
    iconCls : 'locked',
		layout : 'fit',
		width : 460,
		height : 265,
		closeAction : 'hide',
		plain : true,
		modal : true,
		collapsible : false,
		titleCollapse : false,
		maximizable : false,
		draggable : true,
		closable : false,
		resizable : false,
		animateTarget : document.body,
		items : panel,
		buttons : [{
			text : '登录',
			cls : "x-btn-text-icon",
			icon : "/Content/icons/lock_open.png",
			height : 30,
			handler : function() {
				login();
			}
		}, {
			text : '重置',
			cls : "x-btn-text-icon",
			icon : "/Content/icons/arrow_redo.png",
			height : 30,
			handler : function() {
				var loginForm = Ext.getCmp('loginForm');
				loginForm.form.reset();
			}
		},{
            text:'选择皮肤',
            iconCls: 'style_edit',  // <-- icon
            height : 30,
            menu: menu  // assign menu by instance
      }]
	});
	win.show();
	win.on('show', function() {
		setTimeout(function() {
					var account = Ext.getCmp('loginForm').findById('account');
					var password = Ext.getCmp('loginForm').findById('password');
					var c_account = getCookie('msa.login.account');
					account.setValue(c_account);
					if(Ext.isEmpty(c_account)){
						account.focus();
					}else{
						password.focus();
					}
				}, 200);

	}, this);

	/**

	 * 提交登陆请求

	 */

	function login() {
		if (Ext.getCmp('loginForm').form.isValid()) {
      var strusername=Ext.getCmp('loginForm').findById('account').getValue();
      var strpassword=Ext.getCmp('loginForm').findById('password').getValue();
			Ext.Ajax.request({
          method : 'POST',
          waitMsg : '正在验证您的身份,请稍候.....',
          url:'/msa/main.xp',
          success: function(response, options){
          	  var objs = eval(response.responseText);
          	  if(objs[0].errtype=="0"){//完全正确
								setCookie("msa.login.account", strusername);
								setCookie("msa.login.sessionid", objs[0].sessionid);
								window.location.href = '/msa/main.xp?Fun=msaShowTopview';
          	  }else if(objs[0].errtype=="9"){//注册问题
          	  	Ext.MessageBox.alert('提示','您的使用许可已经过期，请联系宝创立即更换许可。400-8892728或者021-51097213');
          	  }else if(objs[0].errtype=="1"){//ip问题
          	  	Ext.MessageBox.alert('提示','该用户不允许在该IP登陆！');
          	  }else{
          	  	  Ext.MessageBox.alert('提示',"登陆失败，用户或密码错误！");
          	  		Ext.getCmp('loginForm').form.reset();
          	  }
          },
          failure : function(response, options) {
           			 Ext.MessageBox.alert('提示',"登陆失败！等会重新登陆");
           			 Ext.getCmp('loginForm').form.reset();
								 account.focus();
								 account.validate();
          },
          params:{
	          Fun:'msaAdminLogon',
		        username:strusername,
		        password:strpassword
          }
		  });
		}
	}
});
