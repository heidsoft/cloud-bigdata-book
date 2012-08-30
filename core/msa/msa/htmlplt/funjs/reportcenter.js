
var reportDataTree = new Ext.tree.TreePanel({
					    	  id: 'reporttreepanel',
					        split: true,
//					        height: 400,
					        minSize: 150,
					        autoScroll: true,   
					        rootVisible: false,
					        lines: false,
					        useArrows: true,
					        loader: new Ext.tree.TreeLoader({
					            dataUrl:'/msa/data/report-tree-data.json'
					        }), 
					        root: new Ext.tree.AsyncTreeNode()
});
					   				  
reportDataTree.on('click', function(n){
					    var sn = this.selModel.selNode || {}; // selNode is null on initial selection					    
					    if(n.leaf){  // ignore clicks on folders and currently selected node 
									if(n.id=='newstyle'){ 
									    mainPanel.loadFame("/msa/main.xp?Fun=msareporttemplate","maintabid3",n.text);
									}else if(n.id=='sort1' ||  n.id=='sort2' ||  
													 n.id=='sort3' ||  n.id=='sort4' ||  
													 n.id=='sort5' ||  n.id=='sort6' ||  
													 n.id=='sort7' ||  n.id=='reporttemplatelist'||
													 n.id=='groupall'||n.id=='userall'){
										  mainPanel.loadFame("/msa/main.xp?Fun=msasortreportlink+type="+n.id,"maintabid3",n.text);   
									}else if(n.id=='sysline' || n.id=='srvline' ||
										       n.id=='userline'|| n.id=='flowline'||
										       n.id=='webline' || n.id=='riskline'){
										  mainPanel.loadFame("/msa/main.xp?Fun=msaReportShowPage+type="+n.id,"maintabid3",n.text);     
									}else{
										  mainPanel.loadFame("/msa/report/"+ n.id+'.htm',"maintabid3",n.text);     
									}
									mainLoadHelpDoc('/msa/help/report.htm');
							}
 });