		var xtzystore;
Ext.onReady(function(){
	var detailEl;
	var rightClick= new Ext.menu.Menu({

							id:'feeds-ctx',
							items: [{
								id:'realidclientlist',
								iconCls:'load-icon',
								text:'删除用户',
								scope: this,
								handler:function(){
									var record = centPanel.getStore().getAt(detailEl);
									var myMatching_id=record.get("Matching_id");
									var actiontype;
									Ext.MessageBox.confirm('提示','你确定要删除此列吗？',function(button,text){
											if(button=="yes"){
												Ext.Ajax.request({
													url:'/msas/main2/main.xp',
													parame:{Fun:'msasMainAction',actiontype:'1',id:myMatching_id} ,
													success:function(){},
													failure:function(){}
												});
											}
									});
								}
							}
						]});
	
	centerstore = new Ext.data.JsonStore({
			        root:'Results',
			        totalProperty:'totalCount',
			        url:'/msas/main2/main.xp',
					baseParams:{Fun:'msasMainAction'},
					fields: [
										{name: 'Matching_id'},
										{name: 'gnmkey'},
										{name: 'sname'},
										{name: 'Rule_id'},
										{name: 'Rule_name'},
										{name: 'Matching_time'},
										{name: 'Net_ending_ip'},
										{name: 'Net_ending_code'},
										{name: 'Net_ending_mac'},
										{name: 'Destination_ip'},
										{name: 'Service_type'},
										{name: 'KeyWord1'},
										{name: 'KeyWord2'},
										{name: 'name'},
										{name: 'Certificate_type'},
										{name: 'ctypes'},
										{name: 'Certificate_code'},
										{name: 'Rule_type'},
										{name: 'Rule_types'},
										{name: 'contenturl'}
							]
			     });
		centerstore.load();
	var centPanel = new Ext.grid.GridPanel({
			        region:'center',
			        store: centerstore,
			        columns: [
									{header: "报警类型", width: 80, sortable: true, dataIndex: 'Rule_type'},
									{header: "触发规则",  width: 80, sortable: true,  dataIndex: 'Rule_name'},
									{header: "触发时间",  width: 100, sortable: true,  dataIndex: 'Matching_time'},
									{id:"sname",header: "上网场所",  width: 120, sortable: true,  dataIndex: 'sname'},
									{header: "IP地址",    width: 80, sortable: true,  dataIndex: 'Net_ending_ip'},
									{header: "MAC地址",   width: 80, sortable: true,  dataIndex: 'Net_ending_mac'},
									{header: "关键字1",   width: 80, sortable: true,  dataIndex: 'KeyWord1'},
									{header: "关键字2",   width: 80, sortable: true,  dataIndex: 'KeyWord2'},
									{header: "证件类型",  width: 80, sortable: true,  dataIndex: 'Certificate_type'},
									{header: "证件号码",  width: 100, sortable: true,  dataIndex: 'Certificate_code'}
							],
			        stripeRows: true,
			        autoExpandColumn: 'sname',
			        width:280,
			        height:screen.height-350,
			        frame:true,
			        title:'布控规则列表',
					collapsible:true,
					draggable:true
			    });
		centPanel.addListener('rowclick',rightClickFn);	
		function rightClickFn(client, rowIndex, e) { 
									rightClick.showAt(e.getXY()); 
								 	detailEl= rowIndex;

                     }       
		
    var northPanel =new Ext.BoxComponent({
			                    region:'north',
			                    el: 'north',
			                    height:24
			      });

	 var treePanel = new Ext.tree.TreePanel({
					    	  id: 'tree-panel',
					        layout:"fit",
					        split: true,
					        height: 350,
					        minSize: 150,
					        autoScroll: true,
					        rootVisible: false,
					        lines: false,
					        useArrows: true,
					        loader: new Ext.tree.TreeLoader({
					            dataUrl:'/msas/test2/rule-tree-data.json'
					        }),
					        
					        root: new Ext.tree.AsyncTreeNode()
					   });
	var westPanel={
					id: 'east-panel',
                    region:'west',
                    collapsible: true,
                    split:true,
                    width: 250,
                    minSize: 200,
                    maxSize: 400,
                    layout:'fit',
                    margins:'0 5 0 0',
                    title:'选择主线',
                    tbar:[{pressed:true,text:"添加"}],
					items:[treePanel]
                    };              
	
	new Ext.Viewport({
	layout:"border",
	items:[
	centPanel,
	westPanel,
	northPanel
		]
	});

				    
});