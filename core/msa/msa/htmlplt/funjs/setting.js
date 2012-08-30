var settingTreePanel = new Ext.tree.TreePanel({
				    	  id: 'setting-tree-panel',
				        split: true,
				        minSize: 150,
				        autoScroll: true,
				        rootVisible: false,
				        lines: false,
				        useArrows: true,
				        root: new Ext.tree.AsyncTreeNode({
                    		expanded: true,
                        children:  [{
																  	text:'参数设置',
																    expanded: true,
																    cls:'feeds-node',
																    children:[{
																	        text:'端口属性',
																	        iconCls: 'feed-icon',
																	        cls:'feed',
																	        id:'ltportinfolist',
																	        leaf:true
																	    },{
																	        text:'系统参数',
																	        iconCls: 'feed-icon',
																	        cls:'feed',
																	        id:'ltappFunction',
																	        leaf:true
																	    },{
																	        text:'邮件分类',
																	        iconCls: 'feed-icon',
																	        cls:'feed',
																	        id:'msamailGroup',
																	        leaf:true
																	    },{
																	        text:'表单分类',
																	        iconCls: 'feed-icon',
																	        cls:'feed',
																	        id:'msaformGroup',
																	        leaf:true
																	    }]
																  }
                        ]
                })
				   });
				  
settingTreePanel.on('click', function(n){
					    	var sn = this.selModel.selNode || {}; 
								if(n.leaf){  
									 if(n.id=='ltportinfolist'){
									     mainPanel.loadFame('/msa/main.xp?Fun=ltportinfolist',"maintabid4",n.text);
								       mainLoadHelpDoc('/msa/help/ltportinfolist.htm');
									 }else if(n.id=='ltappFunction'){
									     mainPanel.loadFame('/msa/main.xp?Fun=ltappFunction',"maintabid4",n.text);
								       mainLoadHelpDoc('/msa/help/ltappFunction.htm');
									 }else if(n.id=='msamailGroup'){
									     mainPanel.loadFame('/msa/main.xp?Fun=msamailGroup',"maintabid4",n.text);
								       mainLoadHelpDoc('/msa/help/msamailGroup.htm');
									 }else if(n.id=='msaformGroup'){
									     mainPanel.loadFame('/msa/main.xp?Fun=msaformGroup',"maintabid4",n.text);
								       mainLoadHelpDoc('/msa/help/msaformGroup.htm');
									 }
//									 else if(n.id=='kernelqos'){
//									     mainPanel.loadFame('/msa/main.xp?Fun=ltbandlink',"maintabid4",n.text);
//								       mainLoadHelpDoc('/msa/help/ltbandlink.htm');
//									 }else if(n.id=='qospathclass'){
//									     mainPanel.loadFame('/msa/main.xp?Fun=ltqosdownlink',"maintabid4",n.text);
//								       mainLoadHelpDoc('/msa/help/ltqosdownlink.htm');
//									 }
					    	}
					  });