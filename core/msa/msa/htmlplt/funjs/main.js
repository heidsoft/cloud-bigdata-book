Ext.BLANK_IMAGE_URL = '/msa/extjs/resources/images/default/s.gif';	  
Ext.onReady(function(){

    Ext.state.Manager.setProvider(new Ext.state.CookieProvider());
    var datacenterwin;
    var usercenterwin;
          
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
									mainPanel.refeshFame();
								}
            }, {
                text: '深蓝风格',
                checked: false,
                group: 'theme',
                checkHandler: function() {
									setTheme(1);
									mainPanel.refeshFame();
								}
            }, {
                text: '浅灰风格',
                checked: false,
                group: 'theme',
                checkHandler: function() {
									setTheme(2);
									mainPanel.refeshFame();
								}
            }, {
                text: '金黄风格',
                checked: false,
                group: 'theme',
                checkHandler: function() {
									setTheme(3);
									mainPanel.refeshFame();
								}
            }
        ]
   });
   
    var sysadminsmenu = new Ext.menu.Menu({
        id: 'sysadminsmainMenu',
        items: [
            // stick any markup in a menu
            '<b class="menu-title">选择一个操作</b>',
            {
                text: '管理员操作',
                handler: function() {
										mainPanel.loadFame('/msa/main.xp?Fun=msaAdminUserList',"maintabid6",'管理员操作');
								    mainLoadHelpDoc('/msa/help/msaAdminUserList.htm');
								}
            }, {
                text: '管理员日志',
                handler: function() {
										mainPanel.loadFame('/msa/manager/admloglist.htm',"maintabid6",'管理员日志');
								    mainLoadHelpDoc('/msa/help/admloglist.htm');
								}
            }, {
                text: '在线管理员',
                handler: function() {
                	  mainPanel.loadFame('/msa/manager/adminlistinfo.htm',"maintabid6",'在线管理员');
								    mainLoadHelpDoc('/msa/help/adminlistinfo.htm');
								}
            }]
   });
    
    var bypasstoolsmenu = new Ext.menu.Menu({
        id: 'bypasstoolsmainMenu',
        items: [{
                text: '软件BYPASS',
                handler: function() {
								}
            }, {
                text: '硬件BYPASS',
                handler: function() {
								}
            }]
    });
   
    var resettoolsmenu = new Ext.menu.Menu({
        id: 'resettoolsmainMenu',
        items: [
//             {
//                text: '缓存重置',
//                handler: function() {
//											function confirmdelgroup1(btn){
//							        if(btn=='yes'){
//							        	    Ext.MessageBox.show({
//																	           msg: '正在缓存重置......',
//																	           progressText: '正在缓存重置....',
//																	           width:450,
//																	           wait:true,
//																	           progress:true,
//																	           waitConfig: {interval:1000},
//																	           icon:'ext-mb-download'
//														}); 
//														Ext.Ajax.request({
//							                      method : 'POST', 
//							                      url:'/msa/main.xp', 
//							                      success: function(response, options) {
//							                      	Ext.MessageBox.hide();
//																			Ext.MessageBox.alert('提示',"已经应用系统缓存成功");
//							                      },
//							                      failure : function() {
//							                      	Ext.MessageBox.hide();
//							                       	Ext.MessageBox.alert('提示','应用系统缓存失败!'); 
//							                      },
//							                      params:{Fun:'msaMemReset'} 
//													 });
//							        }
//									 };
//							     Ext.MessageBox.confirm('Confirm', '您确认要重新应用系统缓存吗?', confirmdelgroup1); 
//								}
//            },
            {
                text: '软件重启',
                handler: function() {
                	function confirmappreset(btn){
					        if(btn=='yes'){
                    var  trynum;	
                    var  trynum1;
                    trynum=0;	
                    trynum1=0;			        
					          Ext.Ajax.request({
						                      method : 'POST', 
						                      url:'/msa/main.xp', 
					                        success: function(response, options) { 
					    													var runner;
					              
																	      var myMask = new Ext.LoadMask(Ext.getBody(),{
																							msg:"正在应用重启,Please wait..."
																				});
																				myMask.show();
																	      
																	      var task = {
																						run: function(){
																							 
																						   Ext.Ajax.request({
																	                      method : 'POST', 
																	                      url:'/msa/main.xp', 
																                        success: function(response, options) { 
																                        	trynum1=trynum1+1;
																    											var retxt=response.responseText;
																    											if(retxt.substring(0,1)=="1" ){
																    												var  strshowtxt;
																    												if(trynum1<11){
																    											  	trynum=0;
																    											  }
																    												trynum=trynum+1;
																    												if(trynum==5){
																    													myMask.hide();
																    													Ext.MessageBox.alert('提示','应用启动成功！'); 
																    												}
																    											}	
																                        }, 
																                        failure : function() {  
																                          runner.stopAll();   
																                          myMask.hide();   
																                         	Ext.MessageBox.alert('提示','应用启动失败！'); 
											                                  },
																                       params:{
																                            Fun:'ltsSysRunCheck'
											 					                       } 
																               });
																						},
																						interval: 2000 // 每30秒一周期
																				};
																				runner = new Ext.util.TaskRunner();
																				runner.start(task);
					                        },
					                        failure : function() {    
					                          Ext.MessageBox.hide();  
					                         	Ext.MessageBox.alert('提示','应用启动失败！'); 
                                  },
 					                        params:{
 					                            Fun:'msaSysApplication'
 					                        } 
												});
						    
					        }
					        };
							     Ext.MessageBox.confirm('Confirm', '您确认要重新启动系统进程吗，需要30秒时间?', confirmappreset); 
								}
            }, {
                text: '硬件重启',
                handler: function() {
											function confirmdelgroup1(btn){
							        if(btn=='yes'){
							        	    Ext.MessageBox.show({
																	           msg: '正在处理在线数据，请稍候......',
																	           progressText: '正在处理在线数据，请稍候....',
																	           width:450,
																	           wait:true,
																	           progress:true,
																	           waitConfig: {interval:1000},
																	           icon:'ext-mb-download'
														});
														Ext.Ajax.request({
							                      method : 'POST', 
							                      url:'/msa/main.xp', 
							                      success: function(response, options) {
																			Ext.MessageBox.alert('提示',"系统重新启动中，在2分钟后重新登录");
							                      },
							                      failure : function() {
							                       	Ext.MessageBox.alert('提示','重新启动系统失败!'); 
							                      },
							                      params:{Fun:'msaSysReset'} 
													 });
							        }
									 };
							     Ext.MessageBox.confirm('Confirm', '您确认要重新启动系统吗?', confirmdelgroup1); 
								}
            }]
    });
   
    var systoolsmenu = new Ext.menu.Menu({
        id: 'systoolsmainMenu',
        items: [
            // stick any markup in a menu
            '<b class="menu-title">选择一个操作</b>',
            {
                text: '启动BYPASS',
                handler: function() {
                	   function confirmbypass(btn){
							        if(btn=='yes'){
							        	    Ext.MessageBox.show({
																	           msg: '正在启动BYPASS......',
																	           progressText: '正在启动BYPASS....',
																	           width:450,
																	           wait:true,
																	           progress:true,
																	           waitConfig: {interval:1000},
																	           icon:'ext-mb-download'
														}); 
														Ext.Ajax.request({
							                      method : 'POST', 
							                      url:'/msa/main.xp', 
							                      success: function(response, options) {
							                      	Ext.MessageBox.hide();
																			Ext.MessageBox.alert('提示',"设备已经进入BYPASS状态");
							                      },
							                      failure : function() {
							                      	Ext.MessageBox.hide();
							                       	Ext.MessageBox.alert('提示','设置BYPASS状态失败!'); 
							                      },
							                      params:{Fun:'msaSoftBypass',bypassType:1} //check 
													 });
							        }
									   };
									   
									   function confirmnobypass(btn){
							        if(btn=='yes'){
														Ext.Ajax.request({
							                      method : 'POST', 
							                      url:'/msa/main.xp', 
							                      success: function(response, options) {
																			Ext.MessageBox.alert('提示',"设备已经进入正常状态");
							                      },
							                      failure : function() {
							                       	Ext.MessageBox.alert('提示','设置BYPASS状态失败!'); 
							                      },
							                      params:{Fun:'msaSoftBypass',bypassType:0} //关闭bypass，进入正常状态 
													 });
							        }
									   };
                	   
                	   Ext.Ajax.request({
							                      method : 'POST', 
							                      url:'/msa/main.xp', 
							                      success: function(response, options) {
																			var retxt=response.responseText;
																    	if(retxt.substring(0,1)=="1" ){//当前是正常状态
																					Ext.MessageBox.confirm('Confirm', '当前设备是正常状态,您确认启动BYPASS吗?', confirmbypass);
																			}else{//当前是bypass
																				  Ext.MessageBox.confirm('Confirm', '当前设备是BYPASS状态,您确认关闭BYPASS吗?', confirmnobypass);
																			}
							                      },
							                      failure : function() {
							                       	Ext.MessageBox.alert('提示','检查BYPASS状态失败!'); 
							                      },
							                      params:{Fun:'msaSoftBypass',bypassType:3} //check 
										 });
                	
								}
            },{
                text: '重置与重启',
                menu: resettoolsmenu
            }, {
                text: '看门狗设置',
                handler: function() {
											mainPanel.loadFame("/msa/main.xp?Fun=ltwatchdog","maintabid6",'看门狗设置');
								      mainLoadHelpDoc('/msa/help/watchdog.htm');
								}
            } ,{
                text: '数据清空',
                handler: function() {
											var meizzTheYear,meizzTheMonth,today,outObject;
											meizzTheYear = new Date().getFullYear();
											meizzTheMonth = new Date().getMonth()+1;
											today=new Date().getDate();
											if(meizzTheMonth<10){meizzTheMonth="0"+meizzTheMonth;}   
											if(today<10){today="0"+today;}
											outObject=meizzTheYear + "-" + meizzTheMonth + "-" + today; 
											//location.href="/msa/main.xp?Fun=sysclean+date="+outObject;
											mainPanel.loadFame("/msa/main.xp?Fun=sysclean+date="+outObject,"maintabid6",'数据清空');
								      mainLoadHelpDoc('/msa/help/sysclean.htm');
								}
            }, {
                text: '设备关机',
                handler: function() {
                	function confirmdelgroup(btn){
							        if(btn=='yes'){
							        	   Ext.MessageBox.show({
																	           msg: '正在处理在线数据，请稍候......',
																	           progressText: '正在处理在线数据，请稍候....',
																	           width:450,
																	           wait:true,
																	           progress:true,
																	           waitConfig: {interval:1000},
																	           icon:'ext-mb-download'
														});
														Ext.Ajax.request({
							                      method : 'POST', 
							                      url:'/msa/main.xp', 
							                      success: function(response, options) {
							                      	Ext.MessageBox.hide();
																			Ext.MessageBox.alert('提示',"系统关闭中，在30秒后可以安全关闭电源");
							                      },
							                      failure : function() {
							                       	Ext.MessageBox.alert('提示','关闭系统失败!'); 
							                       },
							                      params:{Fun:'msaSysShutdown'} 
													 });
							        }
									};
							    Ext.MessageBox.confirm('Confirm', '您确认要关闭设备吗?', confirmdelgroup);
								}
            },{
                text: '每天数据手动整理',
                handler: function() {
                	function confirmDayData(btn){
							        if(btn=='yes'){
							        	    
							        	    Ext.MessageBox.show({
																	           msg: '手动数据每天整理',
																	           progressText: '数据正在整理....',
																	           width:450,
																	           wait:true,
																	           progress:true,
																	           waitConfig: {interval:1000},
																	           icon:'ext-mb-download'
														}); 
							        	    
														Ext.Ajax.request({
											          method : 'POST',
											          url:'/msa/main.xp',
											          success: function(response, options){
											          	      var mypid;
														          	if(response.responseText==-1){
														          		Ext.MessageBox.hide();
														          		Ext.MessageBox.alert("提示","已经有数据整理程序运行，不要重复运行！");
														          		return;
														          	};
														          	mypid=response.responseText;
														          	var runner;
																				var task = {
																						run: function(){
																									Ext.Ajax.request({
																												method : 'POST',
																												url:'/msa/main.xp',
																												success: function(response, options){
																														if(response.responseText==1){
																																Ext.MessageBox.updateProgress(0, '程序运行中 请稍等' );	
																														}else if(response.responseText==0){
																															  Ext.MessageBox.hide();
																																runner.stopAll();
																																Ext.MessageBox.alert("提示","程序执行完毕");
																														}
																												},
																												failure : function(){
																													  Ext.MessageBox.hide();
																														runner.stopAll();
																														Ext.MessageBox.alert('提示',' 请稍后再试');
																												},
																												params:{Fun:'msaIsPidRuning',procpid:mypid}
																									});
																						},
																						interval:2000 // 每30秒一周期
																				};
																				runner = new Ext.util.TaskRunner();
																				runner.start(task);
											          },
											          failure : function(){
											          	Ext.MessageBox.alert("提示","操作失败!");
											          },
											          params:{Fun:'msaStartDayLongProc'}
														});
							        }
									};
							    Ext.MessageBox.confirm('Confirm', '<font color=red>您确认要手动运行每天数据整理程序吗?如果您的系统每天晚上没有关机，请不要手动运行，一天仅允许运行一次，多次运行会造成统计数据重复。在大数据量的环境下，这甚至可能需要几十分钟时间，请耐心等待</font>', confirmDayData);
								}
            }
            , {
                text: '云验证与升级',
                handler: function() {
                	    mainPanel.loadFame("/msa/manager/onlinecheck.htm","maintabid6",'云服务');
								      mainLoadHelpDoc('/msa/help/onlinecheck.htm');
								}
            }, {
                text: '更新使用许可',
                handler: function() {
                	    function confirmupdatereg(btn){
							        if(btn=='yes'){
							        	   mainPanel.loadFame("/msa/main.xp?Fun=ltShowRegister","maintabid6",'更新使用许可');
							        }
									};
							    Ext.MessageBox.confirm('Confirm', '您的许可现在还在有效期内，您确认要立马更新许可吗?如果没有联系厂商请不要操作！', confirmupdatereg);
                	    
								}
            }
        ]
   });
       
     var sysbusmenu = new Ext.menu.Menu({
        id: 'sysbsmainMenu',
        items: [
            // stick any markup in a menu
            '<b class="menu-title">选择一个操作</b>',
            {
                text: '设备部署',
                handler: function() {
										 mainPanel.loadFame('/msa/main.xp?Fun=ltdisposition',"maintabid6",'设备部署');
								     mainLoadHelpDoc('/msa/help/msaEquipment.htm');
								}
            }, 
//            {
//                text: '专家模式',
//                handler: function() {
//										 mainPanel.loadFame('/msa/main.xp?Fun=msaEquipment',"maintabid6",'专家模式');
//								     mainLoadHelpDoc('/msa/help/msaEquipment.htm');
//								}
//            }, 
            {
                text: '网络对象',
                handler: function() {
										 mainPanel.loadFame('/msa/manager/netarea.htm',"maintabid6",'网络对象');
								     mainLoadHelpDoc('/msa/help/netarea.htm');
								}
            }, {
                text: '静态路由',
                handler: function() {
                	  mainPanel.loadFame('/msa/main.xp?Fun=msaroutsetinglink',"maintabid6",'静态路由');
								    mainLoadHelpDoc('/msa/help/routsetinglink.htm');
								}
            }, {
                text: '系统安全策略',
                handler: function() {
                	  mainPanel.loadFame('/msa/main.xp?Fun=msasecurityLink',"maintabid6",'系统安全策略');
								    mainLoadHelpDoc('/msa/help/msasecurityLink.htm');
								}
            }, {
                text: '虚拟服务器',
                handler: function() {
                	  mainPanel.loadFame('/msa/main.xp?Fun=vserver',"maintabid6",'虚拟服务器');
								    mainLoadHelpDoc('/msa/help/vserver.htm');
								}
            }
        ]
   });
  
      var sysusercsmenu = new Ext.menu.Menu({
        id: 'sysusercmainMenu',
        items: [
            // stick any markup in a menu
            '<b class="menu-title">选择一个操作</b>',
            {
                text: '添加新帐号',
                handler: function() {
										 mainPanel.loadFame('/msa/main.xp?Fun=msaShowAddClient&gid=0&pid=0',"maintabid6",'添加新帐号');
								     mainLoadHelpDoc('/msa/help/newuser.htm');
								}
            }, {
                text: '进入帐号中心',
                handler: function() {
										 if(!usercenterwin){
					            usercenterwin = new Ext.Window({
					                id: 'ucmainpanel',
					                title:'MSA&reg账户中心',
					                width:1024,
					                height:768,
					                iconCls: 'house',
					                shim:false,
					                animCollapse:false,
					                constrainHeader:true,
					                closeAction: 'hide',
							            modal: 'true',
							            maximizable:'true',
					                layout: 'border',
					                tbar:topUCBar,
					                items:[uccentertab,ucmainPanel,ucfootBar]
								      });
			               }
			               usercenterwin.show();
			               usercenterwin.maximize();
								}
            },{
                text: '文件账号导入',
                handler: function() {
										mainPanel.loadFame("/msa/users/userimport.htm","maintabid6","文件账号导入");
								}
            }, {
                text: 'AD账号配置',
                handler: function() {
									var myurl="/msa/main.xp?Fun=ltadvarconfig";
		      				mainPanel.loadFame(myurl,"maintabid6",'AD账号配置');
								}
            }, {
                text: 'LDAP账号配置',
                handler: function() {
                	var myurl="/msa/main.xp?Fun=ltldapvarconfig";
		              mainPanel.loadFame(myurl,"maintabid6",'LDAP账号配置');
								}
            }, {
                text: 'RADIUS验证配置',
                handler: function() {
                	var myurl="/msa/main.xp?Fun=msaSetRadius";
		              mainPanel.loadFame(myurl,"maintabid6",'RADIUS验证配置');
								}
            },{
                text: '部门导入',
                handler: function() {
									 mainPanel.loadFame("/msa/users/groupimport.htm","maintabid6",'部门导入');
								}
            }
        ]
   });
  
	var topBar = new Ext.Toolbar({
		region : 'north',
		border : false,
		split : true,
		height : 26,
		minSize : 26,
		maxSize : 26,
		items : [{
					xtype : 'tbtext',
					text : adminuser,
					cls : 'x-btn-text-icon',
					icon : '/Content/icons/user.png',
					disabled : true,
					disabledClass : ''
				}, "->", "-", {
					xtype : "tbbutton",
					minWidth : 80,
					text : "刷新当前页",
					cls : "x-btn-text-icon",
					icon : "/Content/icons/arrow_refresh.png",
					handler : function(btn, e) {
						mainPanel.refeshFame();
					}
				}, "-", {
					xtype : "tbbutton",
					minWidth : 80,
					text : "关闭全部标签",
					cls : "x-btn-text-icon",
					icon : "/Content/icons/stop.png",
					handler : function(btn, e) {
						mainPanel.items.each(function(item) {
									if (item.closable) {
										mainPanel.remove(item, true);
									}
								})

					}
				},{
                xtype : "tbbutton",
                minWidth : 80,
                text: '应用全部规则',
                cls : "x-btn-text-icon",
					      icon : "/Content/icons/key_go.png",
                handler: function() {
                	function confirmappreset1(btn){
					        if(btn=='yes'){
                    var  trynum;	
                    var  trynum1;
                    trynum=0;	
                    trynum1=0;			        
					          Ext.Ajax.request({
						                      method : 'POST', 
						                      url:'/msa/main.xp', 
					                        success: function(response, options) { 
					    													var runner;
					              
																	      var myMask = new Ext.LoadMask(Ext.getBody(),{
																							msg:"正在应用重启,Please wait..."
																				});
																				myMask.show();
																	      
																	      var task = {
																						run: function(){
																							 
																						   Ext.Ajax.request({
																	                      method : 'POST', 
																	                      url:'/msa/main.xp', 
																                        success: function(response, options) { 
																                        	trynum1=trynum1+1;
																    											var retxt=response.responseText;
																    											if(retxt.substring(0,1)=="1" ){
																    												var  strshowtxt;
																    												if(trynum1<11){
																    											  	trynum=0;
																    											  }
																    												trynum=trynum+1;
																    												if(trynum==5){
																    													myMask.hide();
																    													Ext.MessageBox.alert('提示','应用启动成功！'); 
																    												}
																    											}	
																                        }, 
																                        failure : function() {  
																                          runner.stopAll();   
																                          myMask.hide();   
																                         	Ext.MessageBox.alert('提示','应用启动失败！'); 
											                                  },
																                       params:{
																                            Fun:'ltsSysRunCheck'
											 					                       } 
																               });
																						},
																						interval: 2000 // 每30秒一周期
																				};
																				runner = new Ext.util.TaskRunner();
																				runner.start(task);
					                        },
					                        failure : function() {    
					                          Ext.MessageBox.hide();  
					                         	Ext.MessageBox.alert('提示','应用启动失败！'); 
                                  },
 					                        params:{
 					                            Fun:'msaSysApplication'
 					                        } 
												});
						    
					        }
					        };
							    Ext.MessageBox.confirm('Confirm', '您确认要重新启动系统进程吗，需要30秒时间?', confirmappreset1); 
								}
        }, "-", {
					xtype : "tbbutton",
					minWidth : 80,
					text : "数据中心",
					cls : "x-btn-text-icon",
					icon : "/Content/icons/database_go.png",
					handler : function(btn, e) {
					        if(!datacenterwin){
					            datacenterwin = new Ext.Window({
					                id: 'datamainpanel',
					                title:'MSA&reg数据中心',
					                width:1024,
					                height:768,
					                iconCls: 'house',
					                shim:false,
					                animCollapse:false,
					                constrainHeader:true,
					                closeAction: 'hide',
							            modal: 'true',
							            maximizable:'true',
					                layout: 'border',
					                items:[datacentertab,dcmainPanel,dcfootBar]
								      });
			            }
			            datacenterwin.show();
			            datacenterwin.maximize();
                  dcloop(0);
					}
				},"-", {
					xtype : "tbbutton",
					minWidth : 80,
					text : "账户中心",
					cls : "x-btn-text-icon",
					icon : "/Content/icons/user.png",
					menu:sysusercsmenu
				},"-", {
					xtype : "tbbutton",
					minWidth : 80,
					text : "设备部署",
					cls : "x-btn-text-icon",
					icon : "/Content/icons/link_go.png",
					menu:sysbusmenu
				},"-", {
					xtype : "tbbutton",
					minWidth : 80,
					text : "系统工具",
					cls : "x-btn-text-icon",
					icon : "/Content/icons/computer_key.png",
					menu: systoolsmenu
				},"-", {
					xtype : "tbbutton",
					minWidth : 80,
					text : "管理员",
					cls : "x-btn-text-icon",
					icon : "/Content/icons/user.png",
					menu: sysadminsmenu
				},
				"-", {
					xtype : "tbbutton",
					minWidth : 80,
					text : "帮助",
					cls : "x-btn-text-icon",
					icon : "/Content/icons/lightbulb.png",
					handler : function(btn, e) {
						 var mainhelppanel = Ext.getCmp("main-help-panel");
						 //alert(mainhelppanel);
						 //mainhelppanel.expand();
						 mainhelppanel.collapsed ? mainhelppanel.expand() : mainhelppanel.collapse(); 
						 //mainLoadHelpDoc('/msa/help/syssrvlistnow.htm');
					}
				}, 
				"-", {
					xtype : "tbbutton",
					minWidth : 80,
					text : "注销",
					cls : "x-btn-text-icon",
					icon : "/Content/icons/lock_go.png",
					handler : function(btn, e) {
						location.href =  '/msa/main.xp?Fun=msaAdminLogout';
					}
				}, "-", {
					xtype : "tbbutton",
					minWidth : 80,
					text : "选择皮肤",
					cls : "x-btn-text-icon",
					icon : "/Content/icons/asterisk_yellow.png",
					menu: menu
				}]
	});
    
  var TopcontentPanel = {
							id: 'content-panel',
							region: 'north', // this is what makes this panel into a region within the containing layout
							margins: '2 5 5 0',
							collapsible: false,
              collapsed:false,
							border: false,
							height:50,
							layout:'fit',
							items:[
                 new Ext.BoxComponent({ // raw
				                    region:'north',
				                    el: 'north',
				                    height:50
				         })
             ],
             bbar:topBar
	};
  

       
  var footBar = new Ext.ux.StatusBar({
				region : "south",
				margins: '2 2 2 2',
				items : ["->", "<b>MSA&reg 宝创科技版权所有</b>  "]
	});
				
  var viewport = new Ext.Viewport({
            layout:'border',
            items:[
                TopcontentPanel, {
                    region:'east',
                    id:'main-help-panel',
                    title: '帮助',
                    collapsible: true,
                    collapsed:true,
                    split:true,
                    width: 300,
                    minSize: 180,
                    maxSize: 400,
                    layout:'fit',
                    margins:'0 5 0 0',
                    items:
                        new Ext.TabPanel({
                            border:false,
                             id:'helptab-panel',
                            activeTab:0,
                            tabPosition:'top',
                            items:[{
                                id:'content-help-panelmain',
										            title: '主页',
										            autoLoad: {url: '/msa/help/welcome.htm', scope: this},
										            iconCls:'icon-docs',
										            autoScroll: true
                            }]
                        })
                },{
                    region:'west',
                    id:'west-panel',
                    title:'功能导航',
                    split:true,
                    width: 180,
                    minSize: 150,
                    maxSize: 300,
                    collapsible: true,
                    margins:'0 0 0 5',
                    layout:'accordion',
                    layoutConfig:{
                        animate:true
                    },
                    items: [{
                        title:'实时状态',
                        border:false,
                        iconCls:'nav',
                        layout: 'fit',
								    	  id: 'layout-browser-nowview',
								        border: false,
								        split:true,
										    margins: '2 0 5 5',
										    items: [nowtreePanel]
                    },{
                        title:'报告中心',
                        border:false,
                        iconCls:'nav',
                        layout: 'fit',
								    	  id: 'layout-browser-report',
								        border: false,
								        split:true,
										    margins: '2 0 5 5',
										    items: [reportDataTree]
                    },{
                        title:'策略中心',
                        border:false,
                        iconCls:'nav',
                        layout: 'fit',
								    	  id: 'layout-browser-rule',
								        border: false,
								        split:true,
										    margins: '2 0 5 5',
										    items: [ruledataTree]
                    },{
                        title:'参数与对象',
                        border:false,
                        iconCls:'nav',
                        layout: 'fit',
								    	  id: 'layout-browser-setting',
								        border: false,
								        split:true,
										    margins: '2 0 5 5',
										    items: [settingTreePanel]
                    },{
                        title:'网络及系统服务',
                        border:false,
                        iconCls:'nav',
                        layout: 'fit',
								    	  id: 'layout-browser-service',
								        border: false,
								        split:true,
										    margins: '2 0 5 5',
										    items: [serviceTreePanel]
                    }]
                },
                mainPanel,footBar
             ]
  });
   
});
