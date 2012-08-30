					  var ruledataTree = new Ext.tree.TreePanel({
                    id: 'rule-tree-panel',
                    autoScroll: true,
										enableAllCheck:true,
                    rootVisible: false,
                    root: new Ext.tree.AsyncTreeNode({
                        expanded: true,
                        children: [{
																  	text:'权限与服务过滤策略',
																    expanded: true,
																    cls:'feeds-node',
																    children:[{
																        text:'全部规则列表',
																        iconCls: 'feed-icon',
																        cls:'feed',
																        id:'servicepolicyall',
																        leaf:true
																    },{
																        text:'服务权限规则列表',
																        iconCls: 'feed-icon',
																        cls:'feed',
																        id:'servicepolicy1',
																        leaf:true
																    },{
																        text:' 静态防火墙规则',
																        iconCls: 'feed-icon',
																        cls:'feed',
																        id:'servicepolicy2',
																        leaf:true
																    },{
																        text:'用户属性设置规则',
																        iconCls: 'feed-icon',
																        cls:'feed',
																        id:'servicepolicy3',
																        leaf:true
																    },{
																        text:'碎片包安全策略',
																        iconCls: 'feed-icon',
																        cls:'feed',
																        id:'fragsec',
																        leaf:true
																    }]
																  },{
																  	text:'关键字与虚拟身份过滤',
																    expanded: true,
																    cls:'feeds-node',
																    children:[{
																        text:'表单递交内容关键字',
																        iconCls: 'feed-icon',
																        cls:'feed',
																        id:'formkeyword',
																        leaf:true
																    },{
																        text:'网页内容关键字拦截',
																        iconCls: 'feed-icon',
																        cls:'feed',
																        id:'webkeyword',
																        leaf:true
																    },{
																        text:'虚拟身份过滤清单',
																        iconCls: 'feed-icon',
																        cls:'feed',
																        id:'videntityFilter',
																        leaf:true
																    }]
																  },{
																  	text:'对象与元素',
																    expanded: true,
																    cls:'feeds-node',
																    children:[{
																        text:'周时间元素',
																        iconCls: 'feed-icon',
																        cls:'feed',
																        id:'ltschinfolist',
																        leaf:true
																    },{
																        text:'节假日元素',
																        iconCls: 'feed-icon',
																        cls:'feed',
																        id:'ltjjrinfolist',
																        leaf:true
																    },{
																        text:'服务特征自定义',
																        iconCls: 'feed-icon',
																        cls:'feed',
																        id:'lturlsort',
																        leaf:true
																    },{
																        text:'QOS通道元素',
																        iconCls: 'feed-icon',
																        cls:'feed',
																        id:'kernelqos',
																        leaf:true
																    }]
																  }
//																  ,{
//																  	text:'系统安全策略',
//																    expanded: true,
//																    cls:'feeds-node',
//																    children:[{
//																        text:'设置系统安全策略',
//																        iconCls: 'feed-icon',
//																        cls:'feed',
//																        id:'syssecurityset',
//																        leaf:true
//																    }]
//																  }
																  ]
                    })
                });
						// Assign the changeLayout function to be called on tree node click.
					  ruledataTree.on('click', function(n){
					    	var sn = this.selModel.selNode || {}; // selNode is null on initial selection
					    	if(n.leaf){  // ignore clicks on folders and currently selected node 
										 if(n.id=='formkeyword'){
										   mainPanel.loadFame('/msa/main.xp?Fun=editkeyword+type=0',"maintabid4",n.text);
								       mainLoadHelpDoc('/msa/help/formkeyword.htm');
										 }else if(n.id=='webkeyword'){
										   mainPanel.loadFame('/msa/main.xp?Fun=editkeyword+type=1',"maintabid4",n.text);
								       mainLoadHelpDoc('/msa/help/webkeyword.htm');
										 }else if(n.id=='videntityFilter'){
										   mainPanel.loadFame('/msa/main.xp?Fun=videntityFilter',"maintabid4",n.text);
								       mainLoadHelpDoc('/msa/help/videntityFilter.htm');
										 }else if(n.id=='syssecurityset'){
										   mainPanel.loadFame('/msa/main.xp?Fun=msasecurityLink',"maintabid4",n.text);
								       mainLoadHelpDoc('/msa/help/syssecurityset.htm');
										 }else if(n.id=='servicepolicyall'){
										   mainPanel.loadFame('/msa/main.xp?Fun=showServicePolicy+type=0',"maintabid4",n.text);
								       mainLoadHelpDoc('/msa/help/servicepolicy.htm');
										 }
										 else if(n.id=='servicepolicy1'){
										   mainPanel.loadFame('/msa/main.xp?Fun=showServicePolicy+type=1',"maintabid4",n.text);
								       mainLoadHelpDoc('/msa/help/servicepolicy.htm');
										 }else if(n.id=='servicepolicy2'){
										   mainPanel.loadFame('/msa/main.xp?Fun=showServicePolicy+type=2',"maintabid4",n.text);
								       mainLoadHelpDoc('/msa/help/servicepolicy.htm');
										 }else if(n.id=='servicepolicy3'){
										   mainPanel.loadFame('/msa/main.xp?Fun=showServicePolicy+type=3',"maintabid4",n.text);
								       mainLoadHelpDoc('/msa/help/servicepolicy.htm');
										 }else if(n.id=='ltschinfolist'){
									     mainPanel.loadFame('/msa/main.xp?Fun=ltschinfolist',"maintabid4",n.text);
								       mainLoadHelpDoc('/msa/help/ltschinfolist.htm');
									 }else if(n.id=='ltjjrinfolist'){
									     mainPanel.loadFame('/msa/main.xp?Fun=ltjjrinfolist',"maintabid4",n.text);
								       mainLoadHelpDoc('/msa/help/ltjjrinfolist.htm');
									 }else if(n.id=='lturlsort'){
									     mainPanel.loadFame('/msa/main.xp?Fun=lturlsort',"maintabid4",n.text);
								       mainLoadHelpDoc('/msa/help/lturlsort.htm');
									 }else if(n.id=='kernelqos'){
									     mainPanel.loadFame('/msa/main.xp?Fun=ltbandlink',"maintabid4",n.text);
								       mainLoadHelpDoc('/msa/help/ltbandlink.htm');
									 }else if(n.id=='fragsec'){
									     mainPanel.loadFame('/msa/main.xp?Fun=msapacksecurityLink',"maintabid4",n.text);
								       mainLoadHelpDoc('/msa/help/msapacksecurityLink.htm');
									 }
									 
									 
                }
					  });