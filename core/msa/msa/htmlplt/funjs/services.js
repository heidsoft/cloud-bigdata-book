var serviceTreePanel = new Ext.tree.TreePanel({
				    	  id: 'service-tree-panel',
				        split: true,
				        minSize: 150,
				        autoScroll: true,
				        rootVisible: false,
				        lines: false,
				        useArrows: true,
				        root: new Ext.tree.AsyncTreeNode({
                    		expanded: true,
                        children: [
//                        						{
//																        text:'虚拟服务器',
//																        iconCls: 'feed-icon',
//																        cls:'feed',
//																        id:'xnserver',
//																        leaf:true
//																    },
																    {
																        text:'DDNS服务',
																        iconCls: 'feed-icon',
																        cls:'feed',
																        id:'ddnsserver',
																        leaf:true
																    },{
																		        text:'时间服务',
																		        iconCls: 'feed-icon',
																		        cls:'feed',
																		        id:'lttimesetting',
																		        leaf:true
																		}
//																		,{
//																        text:'SSLVPN服务',
//																        iconCls: 'feed-icon',
//																        cls:'feed',
//																        id:'tovpnsetting',
//																        leaf:true
//																    }
																    ]
                })
				   });
				  
serviceTreePanel.on('click', function(n){
					    	var sn = this.selModel.selNode || {}; 
								if(n.leaf){  
									 if(n.id=='tovpnsetting'){
									     mainPanel.loadFame('/msa/main.xp?Fun=tovpnsetting',"maintabid5",n.text);
								       mainLoadHelpDoc('/msa/help/tovpnsetting.htm');
									 }else if(n.id=='ddnsserver'){
									     mainPanel.loadFame('/msa/main.xp?Fun=ddnsset',"maintabid5",n.text);
								       mainLoadHelpDoc('/msa/help/ddnsserver.htm');
									 }else if(n.id=='lttimesetting'){
									     mainPanel.loadFame('/msa/main.xp?Fun=lttimesetting',"maintabid5",n.text);
								       mainLoadHelpDoc('/msa/help/lttimesetting.htm');
									 }
					    	}
					  });