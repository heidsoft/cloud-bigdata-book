
DCMainPanel = function(){
		
    DCMainPanel.superclass.constructor.call(this, {
        id:'dcdocmainbody',
        region:'center',
        margins:'0 5 5 0',
        resizeTabs: true,
        minTabWidth: 110,
        tabWidth: 120,
        enableTabScroll: true,
        activeTab: 0,
        items: {
        	  id:'dcmaintabpanel',
            title: '数据中心概览',
            //autoLoad: {url: '/msa/main/test1.htm', callback: this.initMY, scripts : true,scope: this},
            iconCls:'icon-docs',
            autoScroll: true,
            layout:'fit',
						defaults: {
							bodyStyle:'valign:top'
						},
						items: [dcgkmainpanel]
        }
    });
};

Ext.extend(DCMainPanel, Ext.TabPanel, {

    initEvents : function(){
        DCMainPanel.superclass.initEvents.call(this);
        this.body.on('click', this.onClick, this);
    },
    onClick: function(e, target){
        if(target = e.getTarget('a:not(.exi)', 3)){
            var cls = Ext.fly(target).getAttributeNS('ext', 'cls');
            e.stopEvent();
            if(cls){
                var member = Ext.fly(target).getAttributeNS('ext', 'member');
                this.loadClass(target.href, cls, member);
            }else if(target.className == 'inner-link'){
                this.getActiveTab().scrollToSection(target.href.split('#')[1]);
            }else{
                window.open(target.href);
            }
        }else if(target = e.getTarget('.micon', 2)){
            e.stopEvent();
            var tr = Ext.fly(target.parentNode);
            if(tr.hasClass('expandable')){
                tr.toggleClass('expanded');
            }
        }
    },
	  loadFame : function(href, cls, xtitle){
	  	  
        var id = 'dcmainfun-' + cls;
        var tab = this.getComponent(id);        
        if(tab){
        	  var ifameid= 'dcifame-' + cls;
        	  var autoLoad ="<iframe id='" + ifameid + "' frameborder='no'  scrolling='no'  width='100%'  height='100%'  src='" + href + "'></iframe>";
            this.setActiveTab(tab);
            tab.setTitle(xtitle);
            //tab.update(autoLoad);
           // Ext.getCmp(ifameid).location.href=href;
             document.getElementById(ifameid).src=href;
            //tab.doLayout();
        }else{
            //var autoLoad = {url: href};
            var ifameid= 'dcifame-' + cls;
            var autoLoad ="<iframe id='" + ifameid + "' frameborder='no'  scrolling='no'  width='100%'  height='100%'  src='" + href + "'></iframe>";
            //alert(autoLoad);
            var p = this.add(new DocPanel({
                id: id,
                cclass : cls,
                title: xtitle,
                html: autoLoad
                //autoLoad: autoLoad
//                iconCls: Docs.icons[cls]
            }));
            this.setActiveTab(p);
        }
    },
    refeshFame : function(){
	  	  var tab = this.getActiveTab();
	  	  //alert(tab.id);
	  	  //alert(tab.id.substring(5));
				if(tab.id!='dcmaintabpanel'){
					 var tempid=tab.id.substring(10);
					 var ifameid= 'ifame-' + tempid; 
					 //alert(document.getElementById(ifameid).src);
					 document.getElementById(ifameid).src=document.getElementById(ifameid).src;
				}
    },
	initMY : function(){

	},
	
	doSearch : function(e){

	}
});

 var dcmainPanel = new DCMainPanel();
     
     
//     var datacontentPanel = {
//							id: 'content-panel',
//							region: 'center', // this is what makes this panel into a region within the containing layout
//							margins: '2 5 5 0',
//							border: false,
//							item:'<iframe id="dcmainviewfrm" name="dcmainviewfrm" src="/msa/datacenter/fullcalendar.htm" valign=top width="100%" height="100%" border="0" frameborder="0" scrolling="yes"></iframe>'//iframe div->id
//		 };
     
     var dcdataTree = new Ext.tree.TreePanel({
					    	  id: 'data-tree',
					        title:'数据',
                  width: 220,
                  minSize: 125,
                  maxSize: 400,
        					collapsible: false,
        					cmargins:'0 5 5 5',
						      lines:false,
						      autoScroll:true,
						      collapseFirst:false,
					        split: true,
					        height: 350,
					        minSize: 220,
					        autoScroll: true,
					        rootVisible: false,
					        lines: false,
					        useArrows: true,
					        loader: new Ext.tree.TreeLoader({
					            dataUrl:'/msa/data/data-tree-data.json'
					        }),
					        root: new Ext.tree.AsyncTreeNode()
		 });

		 dcdataTree.on('click', function(n){
					    	var sn = this.selModel.selNode || {}; // selNode is null on initial selection
					    	//if(n.leaf && n.id != sn.id){  // ignore clicks on folders and currently selected node
								if(n.leaf){  // ignore clicks on folders and currently selected node
									 if(n.id=='service_record'){
									    //mainviewfrm.location.href='/msa/main.xp?Fun=msaShowAccessList+showtype=1';//服务访问记录
									    dcmainPanel.loadFame('/msa/main.xp?Fun=msaShowAccessList+showtype=1',"dcmaintabid1",n.text);
									 }else if(n.id=='service_search'){
									 		 var meizzTheYear,meizzTheMonth,today,outObject;
												meizzTheYear = new Date().getFullYear();
												meizzTheMonth = new Date().getMonth()+1;
												today=new Date().getDate();
												if(meizzTheMonth<10){meizzTheMonth="0"+meizzTheMonth;}   
												if(today<10){today="0"+today;}
												outObject=meizzTheYear+"-"+meizzTheMonth+"-"+today; 
												var myday=outObject;
										  	 //服务访问记录搜索
										  	dcmainPanel.loadFame('/msa/main.xp?Fun=msaShowAccessSearch+sdate='+myday.substring(2,10)+'+showtype=1',"dcmaintabid1",n.text);
	                 }else if(n.id=='service_interupt'){
                 	    //服务阻断记录
                 	    dcmainPanel.loadFame('/msa/main.xp?Fun=msaShowAccessList+showtype=2',"dcmaintabid1",n.text);
                   }else if(n.id=='service_interupt_search'){
                   		var meizzTheYear,meizzTheMonth,today,outObject;
												meizzTheYear = new Date().getFullYear();
												meizzTheMonth = new Date().getMonth()+1;
												today=new Date().getDate();
												if(meizzTheMonth<10){meizzTheMonth="0"+meizzTheMonth;}   
												if(today<10){today="0"+today;}
												outObject=meizzTheYear+"-"+meizzTheMonth+"-"+today; 
												var myday=outObject;
										  	//mainviewfrm.location.href='/msa/main.xp?Fun=msaShowAccessSearch+sdate='+myday.substring(2,10)+'+showtype=2';//服务阻断记录搜索
										  	dcmainPanel.loadFame('/msa/main.xp?Fun=msaShowAccessSearch+sdate='+myday.substring(2,10)+'+showtype=2',"dcmaintabid1",n.text);
                   }else if(n.id=='msn_record'){          
                   	  var meizzTheYear,meizzTheMonth,today,outObject;
											meizzTheYear = new Date().getFullYear();
											meizzTheMonth = new Date().getMonth()+1;
											today=new Date().getDate();
											if(meizzTheMonth<10){meizzTheMonth="0"+meizzTheMonth;}   
											if(today<10){today="0"+today;}
											outObject=meizzTheYear+""+meizzTheMonth+""+today;           	  
                      //mainviewfrm.location.href='/msa/main.xp?Fun=ltimshowwebquery+groupid=0+uid=-1+flag=1+host=+sdate='+outObject+'+osort=nosort+orderby=0';//MSN记录     
                      dcmainPanel.loadFame('/msa/main.xp?Fun=ltimshowwebquery+groupid=-1+uid=-1+flag=1+host=+sdate='+outObject+'+osort=nosort+orderby=0',"dcmaintabid1",n.text);
                   }else if(n.id=='yahoo_record'){
                   	  var meizzTheYear,meizzTheMonth,today,outObject;
											meizzTheYear = new Date().getFullYear();
											meizzTheMonth = new Date().getMonth()+1;
											today=new Date().getDate();
											if(meizzTheMonth<10){meizzTheMonth="0"+meizzTheMonth;}   
											if(today<10){today="0"+today;}
											outObject=meizzTheYear+""+meizzTheMonth+""+today;           	  
                      //mainviewfrm.location.href='/msa/main.xp?Fun=ltimshowwebquery+groupid=0+uid=-1+flag=2+host=+sdate='+outObject+'+osort=nosort+orderby=0';//yahoo记录 
                      dcmainPanel.loadFame('/msa/main.xp?Fun=ltimshowwebquery+groupid=-1+uid=-1+flag=2+host=+sdate='+outObject+'+osort=nosort+orderby=0',"dcmaintabid1",n.text);
                   }else if(n.id=='qq_record'){
                   	  var meizzTheYear,meizzTheMonth,today,outObject;
											meizzTheYear = new Date().getFullYear();
											meizzTheMonth = new Date().getMonth()+1;
											today=new Date().getDate();
											if(meizzTheMonth<10){meizzTheMonth="0"+meizzTheMonth;}   
											if(today<10){today="0"+today;}
											outObject=meizzTheYear+""+meizzTheMonth+""+today;  
											//mainviewfrm.location.href='/msa/main.xp?Fun=ltimshowwebquery+groupid=0+uid=-1+flag=3+host=+sdate='+outObject+'+osort=nosort+orderby=0';//QQ记录
											dcmainPanel.loadFame('/msa/main.xp?Fun=ltimshowwebquery+groupid=-1+uid=-1+flag=3+host=+sdate='+outObject+'+osort=nosort+orderby=0',"dcmaintabid1",n.text);
                   }else if(n.id=='taobao_record'){
                   	  var meizzTheYear,meizzTheMonth,today,outObject;
											meizzTheYear = new Date().getFullYear();
											meizzTheMonth = new Date().getMonth()+1;
											today=new Date().getDate();
											if(meizzTheMonth<10){meizzTheMonth="0"+meizzTheMonth;}   
											if(today<10){today="0"+today;}
											outObject=meizzTheYear+""+meizzTheMonth+""+today;  
											//淘宝旺旺记录
											dcmainPanel.loadFame('/msa/main.xp?Fun=ltimshowwebquery+groupid=-1+uid=-1+flag=4+host=+sdate='+outObject+'+osort=nosort+orderby=0',"dcmaintabid1",n.text);
                   }else if(n.id=='fetion_record'){
                   	  var meizzTheYear,meizzTheMonth,today,outObject;
											meizzTheYear = new Date().getFullYear();
											meizzTheMonth = new Date().getMonth()+1;
											today=new Date().getDate();
											if(meizzTheMonth<10){meizzTheMonth="0"+meizzTheMonth;}   
											if(today<10){today="0"+today;}
											outObject=meizzTheYear+""+meizzTheMonth+""+today;  
											dcmainPanel.loadFame('/msa/main.xp?Fun=ltimshowwebquery+groupid=-1+uid=-1+flag=5+host=+sdate='+outObject+'+osort=nosort+orderby=0',"dcmaintabid1",n.text);//飞信记录
                   }else if(n.id=='skype_record'){
                   	  var meizzTheYear,meizzTheMonth,today,outObject;
											meizzTheYear = new Date().getFullYear();
											meizzTheMonth = new Date().getMonth()+1;
											today=new Date().getDate();
											if(meizzTheMonth<10){meizzTheMonth="0"+meizzTheMonth;}   
											if(today<10){today="0"+today;}
											outObject=meizzTheYear+""+meizzTheMonth+""+today;  
											dcmainPanel.loadFame('/msa/main.xp?Fun=ltimshowwebquery+groupid=-1+uid=-1+flag=6+host=+sdate='+outObject+'+osort=nosort+orderby=0',"dcmaintabid1",n.text);//skype记录
                   }else if(n.id==1){
                   	  var meizzTheYear,meizzTheMonth,today,outObject;
											meizzTheYear = new Date().getFullYear();
											meizzTheMonth = new Date().getMonth()+1;
											today=new Date().getDate();
											if(meizzTheMonth<10){meizzTheMonth="0"+meizzTheMonth;}   
											if(today<10){today="0"+today;}
											outObject=meizzTheYear+""+meizzTheMonth+""+today; 
                      dcmainPanel.loadFame('/msa/main.xp?Fun=ltmsafiletranquery+groupid=-1+uid=-1+trantype=1+host=+sdate='+outObject+'+osort=nosort+orderby=0',"dcmaintabid1",n.text);//FTP上传
                   }else if(n.id==2){
                   	  var meizzTheYear,meizzTheMonth,today,outObject;
											meizzTheYear = new Date().getFullYear();
											meizzTheMonth = new Date().getMonth()+1;
											today=new Date().getDate();
											if(meizzTheMonth<10){meizzTheMonth="0"+meizzTheMonth;}   
											if(today<10){today="0"+today;}
											outObject=meizzTheYear+""+meizzTheMonth+""+today; 
											dcmainPanel.loadFame('/msa/main.xp?Fun=ltmsafiletranquery+groupid=-1+uid=-1+trantype=2+host=+sdate='+outObject+'+osort=nosort+orderby=0',"dcmaintabid1",n.text);//FTP下载       
                   }else if(n.id==3){
                   	  var meizzTheYear,meizzTheMonth,today,outObject;
											meizzTheYear = new Date().getFullYear();
											meizzTheMonth = new Date().getMonth()+1;
											today=new Date().getDate();
											if(meizzTheMonth<10){meizzTheMonth="0"+meizzTheMonth;}   
											if(today<10){today="0"+today;}
											outObject=meizzTheYear+""+meizzTheMonth+""+today; 
											dcmainPanel.loadFame('/msa/main.xp?Fun=ltmsafiletranquery+groupid=-1+uid=-1+trantype=3+host=+sdate='+outObject+'+osort=nosort+orderby=0',"dcmaintabid1",n.text);//MSN文件传输记录
                   }else if(n.id==4){
                   	  var meizzTheYear,meizzTheMonth,today,outObject;
											meizzTheYear = new Date().getFullYear();
											meizzTheMonth = new Date().getMonth()+1;
											today=new Date().getDate();
											if(meizzTheMonth<10){meizzTheMonth="0"+meizzTheMonth;}   
											if(today<10){today="0"+today;}
											outObject=meizzTheYear+""+meizzTheMonth+""+today; 
											dcmainPanel.loadFame('/msa/main.xp?Fun=ltmsafiletranquery+groupid=-1+uid=-1+trantype=4+host=+sdate='+outObject+'+osort=nosort+orderby=0',"dcmaintabid1",n.text);////QQ文件传输记录
                   }else if(n.id==5){
                   	  var meizzTheYear,meizzTheMonth,today,outObject;
											meizzTheYear = new Date().getFullYear();
											meizzTheMonth = new Date().getMonth()+1;
											today=new Date().getDate();
											if(meizzTheMonth<10){meizzTheMonth="0"+meizzTheMonth;}   
											if(today<10){today="0"+today;}
											outObject=meizzTheYear+""+meizzTheMonth+""+today; 
											dcmainPanel.loadFame('/msa/main.xp?Fun=ltmsafiletranquery+groupid=-1+uid=-1+trantype=5+host=+sdate='+outObject+'+osort=nosort+orderby=0',"dcmaintabid1",n.text);////表单文件上传记录 upload
                   }else if(n.id==6){
                   	  var meizzTheYear,meizzTheMonth,today,outObject;
											meizzTheYear = new Date().getFullYear();
											meizzTheMonth = new Date().getMonth()+1;
											today=new Date().getDate();
											if(meizzTheMonth<10){meizzTheMonth="0"+meizzTheMonth;}   
											if(today<10){today="0"+today;}
											outObject=meizzTheYear+""+meizzTheMonth+""+today; 
											dcmainPanel.loadFame('/msa/main.xp?Fun=ltmsafiletranquery+groupid=-1+uid=-1+trantype=6+host=+sdate='+outObject+'+osort=nosort+orderby=0',"dcmaintabid1",n.text);//邮件附件记录  smtp
                   }else if(n.id==7){
                   	  var meizzTheYear,meizzTheMonth,today,outObject;
											meizzTheYear = new Date().getFullYear();
											meizzTheMonth = new Date().getMonth()+1;
											today=new Date().getDate();
											if(meizzTheMonth<10){meizzTheMonth="0"+meizzTheMonth;}   
											if(today<10){today="0"+today;}
											outObject=meizzTheYear+""+meizzTheMonth+""+today; 
											dcmainPanel.loadFame('/msa/main.xp?Fun=ltmsafiletranquery+groupid=-1+uid=-1+trantype=7+host=+sdate='+outObject+'+osort=nosort+orderby=0',"dcmaintabid1",n.text);////邮件附件记录pop3
                   }else if(n.id==8){
                   	  var meizzTheYear,meizzTheMonth,today,outObject;
											meizzTheYear = new Date().getFullYear();
											meizzTheMonth = new Date().getMonth()+1;
											today=new Date().getDate();
											if(meizzTheMonth<10){meizzTheMonth="0"+meizzTheMonth;}   
											if(today<10){today="0"+today;}
											outObject=meizzTheYear+""+meizzTheMonth+""+today; 
											dcmainPanel.loadFame('/msa/main.xp?Fun=ltmsafiletranquery+groupid=-1+uid=-1+trantype=8+host=+sdate='+outObject+'+osort=nosort+orderby=0',"dcmaintabid1",n.text);//二 http
                   }else if(n.id=='dayfrom'){
                   	  //当日发送的邮件                  	
	                   	  var meizzTheYear,meizzTheMonth,today,outObject;
												meizzTheYear = new Date().getFullYear();
												meizzTheMonth = new Date().getMonth()+1;
												today=new Date().getDate();
												if(meizzTheMonth<10){meizzTheMonth="0"+meizzTheMonth;}   
												if(today<10){today="0"+today;}
												outObject=meizzTheYear+""+meizzTheMonth+""+today; 
												dcmainPanel.loadFame('/msa/main.xp?Fun=ltmsamail+gid=-1+uid=-1+flags=1+rid=-1+subject=+dip=+from=+to=+date='+outObject+'+btype=unit1+bbyte=+fbtype=unit1++fbbyte=+osort=nosort+orderby=0+mailtype=5',"dcmaintabid1",n.text);                	                
                   }else if(n.id=='dayto'){
                   	//当日接收的邮件
	                   	 var meizzTheYear,meizzTheMonth,today,outObject;
												meizzTheYear = new Date().getFullYear();
												meizzTheMonth = new Date().getMonth()+1;
												today=new Date().getDate();
												if(meizzTheMonth<10){meizzTheMonth="0"+meizzTheMonth;}   
												if(today<10){today="0"+today;}
												outObject=meizzTheYear+""+meizzTheMonth+""+today; 
											  dcmainPanel.loadFame('/msa/main.xp?Fun=ltmsamail+gid=-1+uid=-1+flags=0+rid=-1+subject=+dip=+from=+to=+date='+outObject+'+btype=unit1+bbyte=+fbtype=unit1++fbbyte=+osort=nosort+orderby=0+mailtype=5',"dcmaintabid1",n.text);
                   }else if(n.id=='dayfujian'){
                   	   //当日带附件的邮件
                   	    var meizzTheYear,meizzTheMonth,today,outObject;
												meizzTheYear = new Date().getFullYear();
												meizzTheMonth = new Date().getMonth()+1;
												today=new Date().getDate();
												if(meizzTheMonth<10){meizzTheMonth="0"+meizzTheMonth;}   
												if(today<10){today="0"+today;}
												outObject=meizzTheYear+""+meizzTheMonth+""+today; 
												dcmainPanel.loadFame('/msa/main.xp?Fun=ltmsamail+gid=-1+uid=-1+flags=-1+rid=-1+subject=+dip=+from=+to=+date='+outObject+'+btype=unit1+bbyte=+fbtype=unit2++fbbyte=0+osort=nosort+orderby=0+mailtype=5',"dcmaintabid1",n.text);                 	
                   }else if(n.id=='daybig1m'){
                   	  //邮件大于1M的
                   	  var meizzTheYear,meizzTheMonth,today,outObject;
												meizzTheYear = new Date().getFullYear();
												meizzTheMonth = new Date().getMonth()+1;
												today=new Date().getDate();
												if(meizzTheMonth<10){meizzTheMonth="0"+meizzTheMonth;}   
												if(today<10){today="0"+today;}
												outObject=meizzTheYear+""+meizzTheMonth+""+today; 
												dcmainPanel.loadFame('/msa/main.xp?Fun=ltmsamail+gid=-1+uid=-1+flags=-1+rid=-1+subject=+dip=+from=+to=+date='+outObject+'+btype=unit2+bbyte=1024000+fbtype=unit1++fbbyte=+osort=nosort+orderby=0+mailtype=5',"dcmaintabid1",n.text);                 	
                   }else if(n.id=='mail_backlist'){
												dcmainPanel.loadFame('/msa/main.xp?Fun=ltappbackMail',"dcmaintabid1",n.text);                 	
                   }
                   else if(n.id=='daymeetwarning'){
                   	   //符合报警策略??
                   	    var meizzTheYear,meizzTheMonth,today,outObject;
												meizzTheYear = new Date().getFullYear();
												meizzTheMonth = new Date().getMonth()+1;
												today=new Date().getDate();
												if(meizzTheMonth<10){meizzTheMonth="0"+meizzTheMonth;}   
												if(today<10){today="0"+today;}
												outObject=meizzTheYear+""+meizzTheMonth+""+today; 
												dcmainPanel.loadFame('/msa/main.xp?Fun=ltmsamail+gid=-1+uid=-1+flags=-1+rid=-1+subject=+dip=+from=+to=+date='+outObject+'+btype=unit1+bbyte=+fbtype=unit1++fbbyte=+osort=nosort+orderby=0+mailtype=5',"dcmaintabid1",n.text);                 	
                   }else if(n.id=='allsearch'){
                      var meizzTheYear,meizzTheMonth,today,outObject;
											meizzTheYear = new Date().getFullYear();
											meizzTheMonth = new Date().getMonth()+1;
											today=new Date().getDate();
											if(meizzTheMonth<10){meizzTheMonth="0"+meizzTheMonth;}   
											if(today<10){today="0"+today;}
											outObject=meizzTheYear+""+meizzTheMonth+""+today; 
											dcmainPanel.loadFame('/msa/main.xp?Fun=ltmsamail+gid=-1+uid=-1+flags=-1+rid=-1+subject=+dip=+from=+to=+date='+outObject+'+btype=unit1+bbyte=+fbtype=unit1++fbbyte=+osort=nosort+orderby=0+mailtype=5',"dcmaintabid1",n.text);//全部邮件检索
                   }else if(n.id=='web_td_mail'){
                   	   //当日发送的邮件                  	
	                   	  var meizzTheYear,meizzTheMonth,today,outObject;
												meizzTheYear = new Date().getFullYear();
												meizzTheMonth = new Date().getMonth()+1;
												today=new Date().getDate();
												if(meizzTheMonth<10){meizzTheMonth="0"+meizzTheMonth;}   
												if(today<10){today="0"+today;}
												outObject=meizzTheYear+""+meizzTheMonth+""+today; 
												dcmainPanel.loadFame('/msa/main.xp?Fun=ltmsamail+gid=-1+uid=-1+flags=-1+rid=-1+subject=+dip=+from=+to=+date='+outObject+'+btype=unit1+bbyte=+fbtype=unit1++fbbyte=+osort=nosort+orderby=0+mailtype=4',"dcmaintabid1",n.text);                	                
                   }else if(n.id=='web_td_fj_mail'){
                   	 var meizzTheYear,meizzTheMonth,today,outObject;
												meizzTheYear = new Date().getFullYear();
												meizzTheMonth = new Date().getMonth()+1;
												today=new Date().getDate();
												if(meizzTheMonth<10){meizzTheMonth="0"+meizzTheMonth;}   
												if(today<10){today="0"+today;}
												outObject=meizzTheYear+""+meizzTheMonth+""+today; 
												dcmainPanel.loadFame('/msa/main.xp?Fun=ltmsamail+gid=-1+uid=-1+flags=-1+rid=-1+subject=+dip=+from=+to=+date='+outObject+'+btype=unit1+bbyte=+fbtype=unit2++fbbyte=0+osort=nosort+orderby=0+mailtype=4',"dcmaintabid1",n.text);                 	
                   }else if(n.id=='web_td_big1m_mail'){
                   	  
                   	   var meizzTheYear,meizzTheMonth,today,outObject;
												meizzTheYear = new Date().getFullYear();
												meizzTheMonth = new Date().getMonth()+1;
												today=new Date().getDate();
												if(meizzTheMonth<10){meizzTheMonth="0"+meizzTheMonth;}   
												if(today<10){today="0"+today;}
												outObject=meizzTheYear+""+meizzTheMonth+""+today; 
												dcmainPanel.loadFame('/msa/main.xp?Fun=ltmsamail+gid=-1+uid=-1+flags=-1+rid=-1+subject=+dip=+from=+to=+date='+outObject+'+btype=unit2+bbyte=1024000+fbtype=unit1++fbbyte=+osort=nosort+orderby=0+mailtype=4',"dcmaintabid1",n.text);                 	
                   }else if(n.id=='163_mail_search'){              	
	                   	  var meizzTheYear,meizzTheMonth,today,outObject;
												meizzTheYear = new Date().getFullYear();
												meizzTheMonth = new Date().getMonth()+1;
												today=new Date().getDate();
												if(meizzTheMonth<10){meizzTheMonth="0"+meizzTheMonth;}   
												if(today<10){today="0"+today;}
												outObject=meizzTheYear+""+meizzTheMonth+""+today; 
												dcmainPanel.loadFame('/msa/main.xp?Fun=ltmsamail+gid=-1+uid=-1+flags=1+rid=-1+subject=+dip=+from=+to=+date='+outObject+'+btype=unit1+bbyte=+fbtype=unit1++fbbyte=+osort=nosort+orderby=0+mailtype=4',"dcmaintabid1",n.text);                	                
                   }else if(n.id=='126_mail_search'){
                   	    var meizzTheYear,meizzTheMonth,today,outObject;
												meizzTheYear = new Date().getFullYear();
												meizzTheMonth = new Date().getMonth()+1;
												today=new Date().getDate();
												if(meizzTheMonth<10){meizzTheMonth="0"+meizzTheMonth;}   
												if(today<10){today="0"+today;}
												outObject=meizzTheYear+""+meizzTheMonth+""+today; 
												dcmainPanel.loadFame('/msa/main.xp?Fun=ltmsamail+gid=-1+uid=-1+flags=2+rid=-1+subject=+dip=+from=+to=+date='+outObject+'+btype=unit1+bbyte=+fbtype=unit1++fbbyte=+osort=nosort+orderby=0+mailtype=4',"dcmaintabid1",n.text);                	                
												//dcmainPanel.loadFame('/msa/main.xp?Fun=ltmsamail+gid=-1+uid=-1+flags=2+rid=-1+subject=+dip=+from=+to=+date='+outObject+'+btype=unit1+bbyte=+fbtype=unit2++fbbyte=0+osort=nosort+orderby=0+mailtype=4',"dcmaintabid1",n.text);                 	
                   }else if(n.id=='qq_mail_search'){                   	  
                   	    var meizzTheYear,meizzTheMonth,today,outObject;
												meizzTheYear = new Date().getFullYear();
												meizzTheMonth = new Date().getMonth()+1;
												today=new Date().getDate();
												if(meizzTheMonth<10){meizzTheMonth="0"+meizzTheMonth;}   
												if(today<10){today="0"+today;}
												outObject=meizzTheYear+""+meizzTheMonth+""+today; 
												dcmainPanel.loadFame('/msa/main.xp?Fun=ltmsamail+gid=-1+uid=-1+flags=3+rid=-1+subject=+dip=+from=+to=+date='+outObject+'+btype=unit1+bbyte=+fbtype=unit1++fbbyte=+osort=nosort+orderby=0+mailtype=4',"dcmaintabid1",n.text);                	                
												//dcmainPanel.loadFame('/msa/main.xp?Fun=ltmsamail+gid=-1+uid=-1+flags=3+rid=-1+subject=+dip=+from=+to=+date='+outObject+'+btype=unit1+bbyte=0+fbtype=unit2++fbbyte=0+osort=nosort+orderby=0+mailtype=4',"dcmaintabid1",n.text);                 	                	
                   }else if(n.id=='gmail_mail_search'){
                   	    var meizzTheYear,meizzTheMonth,today,outObject;
												meizzTheYear = new Date().getFullYear();
												meizzTheMonth = new Date().getMonth()+1;
												today=new Date().getDate();
												if(meizzTheMonth<10){meizzTheMonth="0"+meizzTheMonth;}   
												if(today<10){today="0"+today;}
												outObject=meizzTheYear+""+meizzTheMonth+""+today; 
												dcmainPanel.loadFame('/msa/main.xp?Fun=ltmsamail+gid=-1+uid=-1+flags=4+rid=-1+subject=+dip=+from=+to=+date='+outObject+'+btype=unit1+bbyte=+fbtype=unit1++fbbyte=+osort=nosort+orderby=0+mailtype=4',"dcmaintabid1",n.text);//全部邮件检索                  	  
                   }else if(n.id=='yahoo_mail_search'){
                   	    var meizzTheYear,meizzTheMonth,today,outObject;
												meizzTheYear = new Date().getFullYear();
												meizzTheMonth = new Date().getMonth()+1;
												today=new Date().getDate();
												if(meizzTheMonth<10){meizzTheMonth="0"+meizzTheMonth;}   
												if(today<10){today="0"+today;}
												outObject=meizzTheYear+""+meizzTheMonth+""+today; 
												dcmainPanel.loadFame('/msa/main.xp?Fun=ltmsamail+gid=-1+uid=-1+flags=5+rid=-1+subject=+dip=+from=+to=+date='+outObject+'+btype=unit1+bbyte=+fbtype=unit1++fbbyte=+osort=nosort+orderby=0+mailtype=4',"dcmaintabid1",n.text);//全部邮件检索                  	  
                   }else if(n.id=='hotmail_mail_search'){
                   	    var meizzTheYear,meizzTheMonth,today,outObject;
												meizzTheYear = new Date().getFullYear();
												meizzTheMonth = new Date().getMonth()+1;
												today=new Date().getDate();
												if(meizzTheMonth<10){meizzTheMonth="0"+meizzTheMonth;}   
												if(today<10){today="0"+today;}
												outObject=meizzTheYear+""+meizzTheMonth+""+today; 
												dcmainPanel.loadFame('/msa/main.xp?Fun=ltmsamail+gid=-1+uid=-1+flags=6+rid=-1+subject=+dip=+from=+to=+date='+outObject+'+btype=unit1+bbyte=+fbtype=unit1++fbbyte=+osort=nosort+orderby=0+mailtype=4',"dcmaintabid1",n.text);//全部邮件检索                  	  
                   }else if(n.id=='live_mail_search'){
                   	    var meizzTheYear,meizzTheMonth,today,outObject;
												meizzTheYear = new Date().getFullYear();
												meizzTheMonth = new Date().getMonth()+1;
												today=new Date().getDate();
												if(meizzTheMonth<10){meizzTheMonth="0"+meizzTheMonth;}   
												if(today<10){today="0"+today;}
												outObject=meizzTheYear+""+meizzTheMonth+""+today; 
												dcmainPanel.loadFame('/msa/main.xp?Fun=ltmsamail+gid=-1+uid=-1+flags=7+rid=-1+subject=+dip=+from=+to=+date='+outObject+'+btype=unit1+bbyte=+fbtype=unit1++fbbyte=+osort=nosort+orderby=0+mailtype=4',"dcmaintabid1",n.text);//全部邮件检索                  	  
                   }else if(n.id=='sina_mail_search'){
                   	    var meizzTheYear,meizzTheMonth,today,outObject;
												meizzTheYear = new Date().getFullYear();
												meizzTheMonth = new Date().getMonth()+1;
												today=new Date().getDate();
												if(meizzTheMonth<10){meizzTheMonth="0"+meizzTheMonth;}   
												if(today<10){today="0"+today;}
												outObject=meizzTheYear+""+meizzTheMonth+""+today; 
												dcmainPanel.loadFame('/msa/main.xp?Fun=ltmsamail+gid=-1+uid=-1+flags=8+rid=-1+subject=+dip=+from=+to=+date='+outObject+'+btype=unit1+bbyte=+fbtype=unit1++fbbyte=+osort=nosort+orderby=0+mailtype=4',"dcmaintabid1",n.text);//全部邮件检索                  	  
                   }else if(n.id=='139_mail_search'){
                   	    var meizzTheYear,meizzTheMonth,today,outObject;
												meizzTheYear = new Date().getFullYear();
												meizzTheMonth = new Date().getMonth()+1;
												today=new Date().getDate();
												if(meizzTheMonth<10){meizzTheMonth="0"+meizzTheMonth;}   
												if(today<10){today="0"+today;}
												outObject=meizzTheYear+""+meizzTheMonth+""+today; 
												dcmainPanel.loadFame('/msa/main.xp?Fun=ltmsamail+gid=-1+uid=-1+flags=9+rid=-1+subject=+dip=+from=+to=+date='+outObject+'+btype=unit1+bbyte=+fbtype=unit1++fbbyte=+osort=nosort+orderby=0+mailtype=4',"dcmaintabid1",n.text);//全部邮件检索                  	  
                   }else if(n.id=='tom_mail_search'){
                   	    var meizzTheYear,meizzTheMonth,today,outObject;
												meizzTheYear = new Date().getFullYear();
												meizzTheMonth = new Date().getMonth()+1;
												today=new Date().getDate();
												if(meizzTheMonth<10){meizzTheMonth="0"+meizzTheMonth;}   
												if(today<10){today="0"+today;}
												outObject=meizzTheYear+""+meizzTheMonth+""+today; 
												dcmainPanel.loadFame('/msa/main.xp?Fun=ltmsamail+gid=-1+uid=-1+flags=10+rid=-1+subject=+dip=+from=+to=+date='+outObject+'+btype=unit1+bbyte=+fbtype=unit1++fbbyte=+osort=nosort+orderby=0+mailtype=4',"dcmaintabid1",n.text);//全部邮件检索                  	  
                   }else if(n.id=='21cn_mail_search'){
                   	    var meizzTheYear,meizzTheMonth,today,outObject;
												meizzTheYear = new Date().getFullYear();
												meizzTheMonth = new Date().getMonth()+1;
												today=new Date().getDate();
												if(meizzTheMonth<10){meizzTheMonth="0"+meizzTheMonth;}   
												if(today<10){today="0"+today;}
												outObject=meizzTheYear+""+meizzTheMonth+""+today; 
												dcmainPanel.loadFame('/msa/main.xp?Fun=ltmsamail+gid=-1+uid=-1+flags=11+rid=-1+subject=+dip=+from=+to=+date='+outObject+'+btype=unit1+bbyte=+fbtype=unit1++fbbyte=+osort=nosort+orderby=0+mailtype=4',"dcmaintabid1",n.text);//全部邮件检索                  	  
                   }else if(n.id=='sougou_mail_search'){
                   	    var meizzTheYear,meizzTheMonth,today,outObject;
												meizzTheYear = new Date().getFullYear();
												meizzTheMonth = new Date().getMonth()+1;
												today=new Date().getDate();
												if(meizzTheMonth<10){meizzTheMonth="0"+meizzTheMonth;}   
												if(today<10){today="0"+today;}
												outObject=meizzTheYear+""+meizzTheMonth+""+today; 
												dcmainPanel.loadFame('/msa/main.xp?Fun=ltmsamail+gid=-1+uid=-1+flags=12+rid=-1+subject=+dip=+from=+to=+date='+outObject+'+btype=unit1+bbyte=+fbtype=unit1++fbbyte=+osort=nosort+orderby=0+mailtype=4',"dcmaintabid1",n.text);//全部邮件检索                  	  
                   }else if(n.id=='189_mail_search'){
                   	    var meizzTheYear,meizzTheMonth,today,outObject;
												meizzTheYear = new Date().getFullYear();
												meizzTheMonth = new Date().getMonth()+1;
												today=new Date().getDate();
												if(meizzTheMonth<10){meizzTheMonth="0"+meizzTheMonth;}   
												if(today<10){today="0"+today;}
												outObject=meizzTheYear+""+meizzTheMonth+""+today; 
												dcmainPanel.loadFame('/msa/main.xp?Fun=ltmsamail+gid=-1+uid=-1+flags=13+rid=-1+subject=+dip=+from=+to=+date='+outObject+'+btype=unit1+bbyte=+fbtype=unit1++fbbyte=+osort=nosort+orderby=0+mailtype=4',"dcmaintabid1",n.text);//全部邮件检索                  	  
                   }else if(n.id=='yiyou_mail_search'){
                   	    var meizzTheYear,meizzTheMonth,today,outObject;
												meizzTheYear = new Date().getFullYear();
												meizzTheMonth = new Date().getMonth()+1;
												today=new Date().getDate();
												if(meizzTheMonth<10){meizzTheMonth="0"+meizzTheMonth;}   
												if(today<10){today="0"+today;}
												outObject=meizzTheYear+""+meizzTheMonth+""+today; 
												dcmainPanel.loadFame('/msa/main.xp?Fun=ltmsamail+gid=-1+uid=-1+flags=14+rid=-1+subject=+dip=+from=+to=+date='+outObject+'+btype=unit1+bbyte=+fbtype=unit1++fbbyte=+osort=nosort+orderby=0+mailtype=4',"dcmaintabid1",n.text);//全部邮件检索                  	  
                   }else if(n.id=='yeah_mail_search'){
                   	    var meizzTheYear,meizzTheMonth,today,outObject;
												meizzTheYear = new Date().getFullYear();
												meizzTheMonth = new Date().getMonth()+1;
												today=new Date().getDate();
												if(meizzTheMonth<10){meizzTheMonth="0"+meizzTheMonth;}   
												if(today<10){today="0"+today;}
												outObject=meizzTheYear+""+meizzTheMonth+""+today; 
												dcmainPanel.loadFame('/msa/main.xp?Fun=ltmsamail+gid=-1+uid=-1+flags=15+rid=-1+subject=+dip=+from=+to=+date='+outObject+'+btype=unit1+bbyte=+fbtype=unit1++fbbyte=+osort=nosort+orderby=0+mailtype=4',"dcmaintabid1",n.text);//全部邮件检索                           	  
                   }else if(n.id=='souhu_mail_search'){
                   	    var meizzTheYear,meizzTheMonth,today,outObject;
												meizzTheYear = new Date().getFullYear();
												meizzTheMonth = new Date().getMonth()+1;
												today=new Date().getDate();
												if(meizzTheMonth<10){meizzTheMonth="0"+meizzTheMonth;}   
												if(today<10){today="0"+today;}
												outObject=meizzTheYear+""+meizzTheMonth+""+today; 
												dcmainPanel.loadFame('/msa/main.xp?Fun=ltmsamail+gid=-1+uid=-1+flags=16+rid=-1+subject=+dip=+from=+to=+date='+outObject+'+btype=unit1+bbyte=+fbtype=unit1++fbbyte=+osort=nosort+orderby=0+mailtype=4',"dcmaintabid1",n.text);//全部邮件检索                         	  
                   }else if(n.id=='fox_mail_search'){
                   	    var meizzTheYear,meizzTheMonth,today,outObject;
												meizzTheYear = new Date().getFullYear();
												meizzTheMonth = new Date().getMonth()+1;
												today=new Date().getDate();
												if(meizzTheMonth<10){meizzTheMonth="0"+meizzTheMonth;}   
												if(today<10){today="0"+today;}
												outObject=meizzTheYear+""+meizzTheMonth+""+today; 
												dcmainPanel.loadFame('/msa/main.xp?Fun=ltmsamail+gid=-1+uid=-1+flags=17+rid=-1+subject=+dip=+from=+to=+date='+outObject+'+btype=unit1+bbyte=+fbtype=unit1++fbbyte=+osort=nosort+orderby=0+mailtype=4',"dcmaintabid1",n.text);//全部邮件检索            	  
                   }else if(n.id=='liantong_mail_search'){
                   	    var meizzTheYear,meizzTheMonth,today,outObject;
												meizzTheYear = new Date().getFullYear();
												meizzTheMonth = new Date().getMonth()+1;
												today=new Date().getDate();
												if(meizzTheMonth<10){meizzTheMonth="0"+meizzTheMonth;}   
												if(today<10){today="0"+today;}
												outObject=meizzTheYear+""+meizzTheMonth+""+today; 
												dcmainPanel.loadFame('/msa/main.xp?Fun=ltmsamail+gid=-1+uid=-1+flags=18+rid=-1+subject=+dip=+from=+to=+date='+outObject+'+btype=unit1+bbyte=+fbtype=unit1++fbbyte=+osort=nosort+orderby=0+mailtype=4',"dcmaintabid1",n.text);//全部邮件检索                        	  
                   }else if(n.id=='263_mail_search'){
                   	    var meizzTheYear,meizzTheMonth,today,outObject;
												meizzTheYear = new Date().getFullYear();
												meizzTheMonth = new Date().getMonth()+1;
												today=new Date().getDate();
												if(meizzTheMonth<10){meizzTheMonth="0"+meizzTheMonth;}   
												if(today<10){today="0"+today;}
												outObject=meizzTheYear+""+meizzTheMonth+""+today; 
												dcmainPanel.loadFame('/msa/main.xp?Fun=ltmsamail+gid=-1+uid=-1+flags=19+rid=-1+subject=+dip=+from=+to=+date='+outObject+'+btype=unit1+bbyte=+fbtype=unit1++fbbyte=+osort=nosort+orderby=0+mailtype=4',"dcmaintabid1",n.text);//全部邮件检索                 	  
                   }else if(n.id=='web_mail_search'){
                   	   var meizzTheYear,meizzTheMonth,today,outObject;
												meizzTheYear = new Date().getFullYear();
												meizzTheMonth = new Date().getMonth()+1;
												today=new Date().getDate();
												if(meizzTheMonth<10){meizzTheMonth="0"+meizzTheMonth;}   
												if(today<10){today="0"+today;}
												outObject=meizzTheYear+""+meizzTheMonth+""+today; 
												dcmainPanel.loadFame('/msa/main.xp?Fun=ltmsamail+gid=-1+uid=-1+flags=-1+rid=-1+subject=+dip=+from=+to=+date='+outObject+'+btype=unit1+bbyte=+fbtype=unit1++fbbyte=+osort=nosort+orderby=0+mailtype=4',"dcmaintabid1",n.text);//全部邮件检索                  	  
                   }else if(n.id=='td_sub_form'){
                   	   var meizzTheYear,meizzTheMonth,today,outObject;
												meizzTheYear = new Date().getFullYear();
												meizzTheMonth = new Date().getMonth()+1;
												today=new Date().getDate();
												if(meizzTheMonth<10){meizzTheMonth="0"+meizzTheMonth;}   
												if(today<10){today="0"+today;}
												outObject=meizzTheYear+""+meizzTheMonth+""+today; 
                   	  	dcmainPanel.loadFame('/msa/main.xp?Fun=ltmsaform+gid=-1+uid=-1+url=+dip=+date='+outObject+'+btype=unit1+bbyte=+fbtype=unit1++fbbyte=+osort=nosort+orderby=0+matype=-1',"dcmaintabid1",n.text);             	             	  
                   }else if(n.id=='td_fj_form'){
                   	  var meizzTheYear,meizzTheMonth,today,outObject;
												meizzTheYear = new Date().getFullYear();
												meizzTheMonth = new Date().getMonth()+1;
												today=new Date().getDate();
												if(meizzTheMonth<10){meizzTheMonth="0"+meizzTheMonth;}   
												if(today<10){today="0"+today;}
												outObject=meizzTheYear+""+meizzTheMonth+""+today; 
                   	  dcmainPanel.loadFame('/msa/main.xp?Fun=ltmsaform+gid=-1+uid=-1+url=+dip=+date='+outObject+'+btype=unit1+bbyte=+fbtype=unit2++fbbyte=0+osort=nosort+orderby=0+matype=-1',"dcmaintabid1",n.text);             	             	  
                      //mainviewfrm.location.href='/msa/datacenter/tdfjform.htm';//当日带附件的表单
                   }else if(n.id=='td_big1m_form'){
                   	  var meizzTheYear,meizzTheMonth,today,outObject;
												meizzTheYear = new Date().getFullYear();
												meizzTheMonth = new Date().getMonth()+1;
												today=new Date().getDate();
												if(meizzTheMonth<10){meizzTheMonth="0"+meizzTheMonth;}   
												if(today<10){today="0"+today;}
												outObject=meizzTheYear+""+meizzTheMonth+""+today; 
                   	  dcmainPanel.loadFame('/msa/main.xp?Fun=ltmsaform+gid=-1+uid=-1+url=+dip=+date='+outObject+'+btype=unit2+bbyte=1024000+fbtype=unit1++fbbyte=+osort=nosort+orderby=0+matype=-1',"dcmaintabid1",n.text);           	             	  
                     // mainviewfrm.location.href='/msa/datacenter/tdbig1mform.htm';//当日大于1M的表单
                   }else if(n.id=='td_bbs_form'){
                   	  var meizzTheYear,meizzTheMonth,today,outObject;
												meizzTheYear = new Date().getFullYear();
												meizzTheMonth = new Date().getMonth()+1;
												today=new Date().getDate();
												if(meizzTheMonth<10){meizzTheMonth="0"+meizzTheMonth;}   
												if(today<10){today="0"+today;}
												outObject=meizzTheYear+""+meizzTheMonth+""+today; 
                   	  dcmainPanel.loadFame('/msa/main.xp?Fun=ltmsaform+gid=-1+uid=-1+url=bbs+dip=+date='+outObject+'+btype=unit1+bbyte=+fbtype=unit1++fbbyte=+osort=nosort+orderby=0+matype=-1',"dcmaintabid1",n.text);           	             	  
                   	  //mainviewfrm.location.href='/msa/datacenter/tdbbsform.htm';//当日BBS相关的表单
                   }else if(n.id=='td_login_form'){
                   	  var meizzTheYear,meizzTheMonth,today,outObject;
												meizzTheYear = new Date().getFullYear();
												meizzTheMonth = new Date().getMonth()+1;
												today=new Date().getDate();
												if(meizzTheMonth<10){meizzTheMonth="0"+meizzTheMonth;}   
												if(today<10){today="0"+today;}
												outObject=meizzTheYear+""+meizzTheMonth+""+today; 
                   	  dcmainPanel.loadFame('/msa/main.xp?Fun=ltmsaform+gid=-1+uid=-1+url=log+dip=+date='+outObject+'+btype=unit1+bbyte=+fbtype=unit1++fbbyte=+osort=nosort+orderby=0+matype=-1',"dcmaintabid1",n.text);          	             	  
                   	  //mainviewfrm.location.href='/msa/datacenter/tdloginform.htm';//当日登录相关的表单
                   }else if(n.id=='td_webmail_form'){
                   	 var meizzTheYear,meizzTheMonth,today,outObject;
												meizzTheYear = new Date().getFullYear();
												meizzTheMonth = new Date().getMonth()+1;
												today=new Date().getDate();
												if(meizzTheMonth<10){meizzTheMonth="0"+meizzTheMonth;}   
												if(today<10){today="0"+today;}
												outObject=meizzTheYear+""+meizzTheMonth+""+today; 
                   	  dcmainPanel.loadFame('/msa/main.xp?Fun=ltmsaform+gid=-1+uid=-1+url=mail+dip=+date='+outObject+'+btype=unit1+bbyte=+fbtype=unit1++fbbyte=+osort=nosort+orderby=0+matype=-1',"dcmaintabid1",n.text);           	             	  
                     // mainviewfrm.location.href='/msa/datacenter/tdwebmailform.htm';//当日webmail相关的表单
                   }else if(n.id=='form_content_search'){
                   	  var meizzTheYear,meizzTheMonth,today,outObject;
												meizzTheYear = new Date().getFullYear();
												meizzTheMonth = new Date().getMonth()+1;
												today=new Date().getDate();
												if(meizzTheMonth<10){meizzTheMonth="0"+meizzTheMonth;}   
												if(today<10){today="0"+today;}
												outObject=meizzTheYear+""+meizzTheMonth+""+today; 
                   	   dcmainPanel.loadFame('/msa/main.xp?Fun=ltmsaform+gid=-1+uid=-1+url=+dip=+date='+outObject+'+btype=unit1+bbyte=+fbtype=unit1++fbbyte=+osort=nosort+orderby=0+matype=-1',"dcmaintabid1",n.text);//全部邮件检索                  	             	  
                   }else if(n.id=='key_list_baidu'){
	                   	  var meizzTheYear,meizzTheMonth,today,outObject;
												meizzTheYear = new Date().getFullYear();
												meizzTheMonth = new Date().getMonth()+1;
												today=new Date().getDate();
												if(meizzTheMonth<10){meizzTheMonth="0"+meizzTheMonth;}   
												if(today<10){today="0"+today;}
												outObject=meizzTheYear+""+meizzTheMonth+""+today; 
												dcmainPanel.loadFame('/msa/main.xp?Fun=ltmsasearchkeyquery+groupid=-1+uid=-1+searchkey=+host=1+sdate='+outObject+'+osort=nosort+orderby=0',"dcmaintabid1",n.text);//关键字列表										
                   }else if(n.id=='key_list_google'){
	                   	  var meizzTheYear,meizzTheMonth,today,outObject;
												meizzTheYear = new Date().getFullYear();
												meizzTheMonth = new Date().getMonth()+1;
												today=new Date().getDate();
												if(meizzTheMonth<10){meizzTheMonth="0"+meizzTheMonth;}   
												if(today<10){today="0"+today;}
												outObject=meizzTheYear+""+meizzTheMonth+""+today; 
												dcmainPanel.loadFame('/msa/main.xp?Fun=ltmsasearchkeyquery+groupid=-1+uid=-1+searchkey=+host=2+sdate='+outObject+'+osort=nosort+orderby=0',"dcmaintabid1",n.text);;//关键字列表										
                   }else if(n.id=='key_list_sogou'){
	                   	  var meizzTheYear,meizzTheMonth,today,outObject;
												meizzTheYear = new Date().getFullYear();
												meizzTheMonth = new Date().getMonth()+1;
												today=new Date().getDate();
												if(meizzTheMonth<10){meizzTheMonth="0"+meizzTheMonth;}   
												if(today<10){today="0"+today;}
												outObject=meizzTheYear+""+meizzTheMonth+""+today; 
												dcmainPanel.loadFame('/msa/main.xp?Fun=ltmsasearchkeyquery+groupid=-1+uid=-1+searchkey=+host=3+sdate='+outObject+'+osort=nosort+orderby=0',"dcmaintabid1",n.text);;//关键字列表										
                   }else if(n.id=='key_list_yahoo'){
	                   	  var meizzTheYear,meizzTheMonth,today,outObject;
												meizzTheYear = new Date().getFullYear();
												meizzTheMonth = new Date().getMonth()+1;
												today=new Date().getDate();
												if(meizzTheMonth<10){meizzTheMonth="0"+meizzTheMonth;}   
												if(today<10){today="0"+today;}
												outObject=meizzTheYear+""+meizzTheMonth+""+today; 
												dcmainPanel.loadFame('/msa/main.xp?Fun=ltmsasearchkeyquery+groupid=-1+uid=-1+searchkey=+host=4+sdate='+outObject+'+osort=nosort+orderby=0',"dcmaintabid1",n.text);;//关键字列表										
                   }else if(n.id=='key_list_soso'){
	                   	  var meizzTheYear,meizzTheMonth,today,outObject;
												meizzTheYear = new Date().getFullYear();
												meizzTheMonth = new Date().getMonth()+1;
												today=new Date().getDate();
												if(meizzTheMonth<10){meizzTheMonth="0"+meizzTheMonth;}   
												if(today<10){today="0"+today;}
												outObject=meizzTheYear+""+meizzTheMonth+""+today; 
												dcmainPanel.loadFame('/msa/main.xp?Fun=ltmsasearchkeyquery+groupid=-1+uid=-1+searchkey=+host=5+sdate='+outObject+'+osort=nosort+orderby=0',"dcmaintabid1",n.text);;//关键字列表										
                   }else if(n.id=='key_list_gougou'){
	                   	  var meizzTheYear,meizzTheMonth,today,outObject;
												meizzTheYear = new Date().getFullYear();
												meizzTheMonth = new Date().getMonth()+1;
												today=new Date().getDate();
												if(meizzTheMonth<10){meizzTheMonth="0"+meizzTheMonth;}   
												if(today<10){today="0"+today;}
												outObject=meizzTheYear+""+meizzTheMonth+""+today; 
												dcmainPanel.loadFame('/msa/main.xp?Fun=ltmsasearchkeyquery+groupid=-1+uid=-1+searchkey=+host=6+sdate='+outObject+'+osort=nosort+orderby=0',"dcmaintabid1",n.text);;//关键字列表										
                   }else if(n.id=='key_list_bing'){
	                   	  var meizzTheYear,meizzTheMonth,today,outObject;
												meizzTheYear = new Date().getFullYear();
												meizzTheMonth = new Date().getMonth()+1;
												today=new Date().getDate();
												if(meizzTheMonth<10){meizzTheMonth="0"+meizzTheMonth;}   
												if(today<10){today="0"+today;}
												outObject=meizzTheYear+""+meizzTheMonth+""+today; 
												dcmainPanel.loadFame('/msa/main.xp?Fun=ltmsasearchkeyquery+groupid=-1+uid=-1+searchkey=+host=7+sdate='+outObject+'+osort=nosort+orderby=0',"dcmaintabid1",n.text);;//关键字列表										
                   }else if(n.id=='key_list_youdao'){
	                   	  var meizzTheYear,meizzTheMonth,today,outObject;
												meizzTheYear = new Date().getFullYear();
												meizzTheMonth = new Date().getMonth()+1;
												today=new Date().getDate();
												if(meizzTheMonth<10){meizzTheMonth="0"+meizzTheMonth;}   
												if(today<10){today="0"+today;}
												outObject=meizzTheYear+""+meizzTheMonth+""+today; 
												dcmainPanel.loadFame('/msa/main.xp?Fun=ltmsasearchkeyquery+groupid=-1+uid=-1+searchkey=+host=8+sdate='+outObject+'+osort=nosort+orderby=0',"dcmaintabid1",n.text);;//关键字列表										
                   }else if(n.id=='key_list'){
                   	  var meizzTheYear,meizzTheMonth,today,outObject;
											meizzTheYear = new Date().getFullYear();
											meizzTheMonth = new Date().getMonth()+1;
											today=new Date().getDate();
											if(meizzTheMonth<10){meizzTheMonth="0"+meizzTheMonth;}   
											if(today<10){today="0"+today;}
											outObject=meizzTheYear+""+meizzTheMonth+""+today; 
											dcmainPanel.loadFame('/msa/main.xp?Fun=ltmsasearchkeyquery+groupid=-1+uid=-1+searchkey=+host=+sdate='+outObject+'+osort=nosort+orderby=0',"dcmaintabid1",n.text);//关键字列表
                   }else if(n.id=='smtp_send_msavclog'){
	                   	  var meizzTheYear,meizzTheMonth,today,outObject;
												meizzTheYear = new Date().getFullYear();										
												meizzTheMonth = new Date().getMonth()+1;
												today=new Date().getDate();
												if(meizzTheMonth<10){meizzTheMonth="0"+meizzTheMonth;}   
												if(today<10){today="0"+today;}
												outObject=meizzTheYear+""+meizzTheMonth+""+today; 
												dcmainPanel.loadFame('/msa/main.xp?Fun=ltmsavcloglink+groupid=-1+uid=-1+vctype=1+vccon=+date='+outObject+'+osort=nosort+orderby=0',"dcmaintabid1",n.text);
                   }else if(n.id=='smtp_accept_msavclog'){
	                   	  var meizzTheYear,meizzTheMonth,today,outObject;
												meizzTheYear = new Date().getFullYear();										
												meizzTheMonth = new Date().getMonth()+1;
												today=new Date().getDate();
												if(meizzTheMonth<10){meizzTheMonth="0"+meizzTheMonth;}   
												if(today<10){today="0"+today;}
												outObject=meizzTheYear+""+meizzTheMonth+""+today; 
												dcmainPanel.loadFame('/msa/main.xp?Fun=ltmsavcloglink+groupid=-1+uid=-1+vctype=2+vccon=+date='+outObject+'+osort=nosort+orderby=0',"dcmaintabid1",n.text);
                   }else if(n.id=='pop3_login_msavclog'){
	                   	  var meizzTheYear,meizzTheMonth,today,outObject;
												meizzTheYear = new Date().getFullYear();										
												meizzTheMonth = new Date().getMonth()+1;
												today=new Date().getDate();
												if(meizzTheMonth<10){meizzTheMonth="0"+meizzTheMonth;}   
												if(today<10){today="0"+today;}
												outObject=meizzTheYear+""+meizzTheMonth+""+today; 
												dcmainPanel.loadFame('/msa/main.xp?Fun=ltmsavcloglink+groupid=-1+uid=-1+vctype=3+vccon=+date='+outObject+'+osort=nosort+orderby=0',"dcmaintabid1",n.text);
                   }else if(n.id=='qq_login_msavclog'){
	                   	  var meizzTheYear,meizzTheMonth,today,outObject;
												meizzTheYear = new Date().getFullYear();										
												meizzTheMonth = new Date().getMonth()+1;
												today=new Date().getDate();
												if(meizzTheMonth<10){meizzTheMonth="0"+meizzTheMonth;}   
												if(today<10){today="0"+today;}
												outObject=meizzTheYear+""+meizzTheMonth+""+today; 
												dcmainPanel.loadFame('/msa/main.xp?Fun=ltmsavcloglink+groupid=-1+uid=-1+vctype=4+vccon=+date='+outObject+'+osort=nosort+orderby=0',"dcmaintabid1",n.text);
                   }else if(n.id=='qq_exit_msavclog'){
	                   	  var meizzTheYear,meizzTheMonth,today,outObject;
												meizzTheYear = new Date().getFullYear();										
												meizzTheMonth = new Date().getMonth()+1;
												today=new Date().getDate();
												if(meizzTheMonth<10){meizzTheMonth="0"+meizzTheMonth;}   
												if(today<10){today="0"+today;}
												outObject=meizzTheYear+""+meizzTheMonth+""+today; 
												dcmainPanel.loadFame('/msa/main.xp?Fun=ltmsavcloglink+groupid=-1+uid=-1+vctype=5+vccon=+date='+outObject+'+osort=nosort+orderby=0',"dcmaintabid1",n.text);
                   }else if(n.id=='telnet_login_msavclog'){
	                   	  var meizzTheYear,meizzTheMonth,today,outObject;
												meizzTheYear = new Date().getFullYear();										
												meizzTheMonth = new Date().getMonth()+1;
												today=new Date().getDate();
												if(meizzTheMonth<10){meizzTheMonth="0"+meizzTheMonth;}   
												if(today<10){today="0"+today;}
												outObject=meizzTheYear+""+meizzTheMonth+""+today; 
												dcmainPanel.loadFame('/msa/main.xp?Fun=ltmsavcloglink+groupid=-1+uid=-1+vctype=6+vccon=+date='+outObject+'+osort=nosort+orderby=0',"dcmaintabid1",n.text);
                   }else if(n.id=='ftp_login_msavclog'){
	                   	  var meizzTheYear,meizzTheMonth,today,outObject;
												meizzTheYear = new Date().getFullYear();										
												meizzTheMonth = new Date().getMonth()+1;
												today=new Date().getDate();
												if(meizzTheMonth<10){meizzTheMonth="0"+meizzTheMonth;}   
												if(today<10){today="0"+today;}
												outObject=meizzTheYear+""+meizzTheMonth+""+today; 
												dcmainPanel.loadFame('/msa/main.xp?Fun=ltmsavcloglink+groupid=-1+uid=-1+vctype=7+vccon=+date='+outObject+'+osort=nosort+orderby=0',"dcmaintabid1",n.text);
                   }else if(n.id=='ftp_exit_msavclog'){
	                   	  var meizzTheYear,meizzTheMonth,today,outObject;
												meizzTheYear = new Date().getFullYear();										
												meizzTheMonth = new Date().getMonth()+1;
												today=new Date().getDate();
												if(meizzTheMonth<10){meizzTheMonth="0"+meizzTheMonth;}   
												if(today<10){today="0"+today;}
												outObject=meizzTheYear+""+meizzTheMonth+""+today; 
												dcmainPanel.loadFame('/msa/main.xp?Fun=ltmsavcloglink+groupid=-1+uid=-1+vctype=8+vccon=+date='+outObject+'+osort=nosort+orderby=0',"dcmaintabid1",n.text);
                   }else if(n.id=='msavclog'){
                   	  var meizzTheYear,meizzTheMonth,today,outObject;
											meizzTheYear = new Date().getFullYear();										
											meizzTheMonth = new Date().getMonth()+1;
											today=new Date().getDate();
											if(meizzTheMonth<10){meizzTheMonth="0"+meizzTheMonth;}   
											if(today<10){today="0"+today;}
											outObject=meizzTheYear+""+meizzTheMonth+""+today; 
											dcmainPanel.loadFame('/msa/main.xp?Fun=ltmsavcloglink+groupid=-1+uid=-1+vctype=0+vccon=+date='+outObject+'+osort=nosort+orderby=0',"dcmaintabid1",n.text);
                   }else if(n.id=='service_huizong_record'){
                   	    var meizzTheYear,meizzTheMonth,today,outObject;
	                   	  var mydate;
												meizzTheYear = new Date().getFullYear();
												meizzTheMonth = new Date().getMonth()+1;
												today=new Date().getDate();
												if(meizzTheMonth<10){meizzTheMonth="0"+meizzTheMonth;}   
												if(today<10){today="0"+today;}
												outObject='msasrvreport'+meizzTheYear+meizzTheMonth+today;      
												dcmainPanel.loadFame('/msa/main.xp?Fun=ltshowservicequery+groupid=-1+uid=-1+service=-1+work=0+tabname='+outObject+'+tabtype='+1+'+btype=unit1+bbyte=+osort=nosort+orderby=0',"dcmaintabid1",n.text);
                   }else if(n.id=='http_huizong_record'){
                   	    var meizzTheYear,meizzTheMonth,today,outObject;
	                   	  var mydate;
												meizzTheYear = new Date().getFullYear();
												meizzTheMonth = new Date().getMonth()+1;
												today=new Date().getDate();
												if(meizzTheMonth<10){meizzTheMonth="0"+meizzTheMonth;}   
												if(today<10){today="0"+today;}
												outObject='msauserhttpreport'+meizzTheYear+meizzTheMonth+today;  
												dcmainPanel.loadFame('/msa/main.xp?Fun=ltwebshowwebsitequery+groupid=-1+uid=-1+host=+service=-1+work=0+tabname='+outObject+'+tabtype='+1+'+btype=unit1+bbyte=+osort=nosort+orderby=0',"dcmaintabid1",n.text);
                   }else if(n.id=='mail_export'){
                   	    var meizzTheYear,meizzTheMonth,today,outObject;
												meizzTheYear = new Date().getFullYear();
												meizzTheMonth = new Date().getMonth()+1;
												today=new Date().getDate();
												if(meizzTheMonth<10){meizzTheMonth="0"+meizzTheMonth;}   
												if(today<10){today="0"+today;}
												outObject=meizzTheYear+""+meizzTheMonth+""+today; 
												dcmainPanel.loadFame('/msa/main.xp?Fun=ltmsamailExport+gid=-1+uid=-1+flags=-1+rid=-1+subject=+dip=+from=+to=+date='+outObject+'+btype=unit1+bbyte=+fbtype=unit1++fbbyte=+osort=nosort+orderby=0+mailtype=5',"dcmaintabid1",n.text);
                   }else if(n.id=='form_export'){
                   	   var meizzTheYear,meizzTheMonth,today,outObject;
												meizzTheYear = new Date().getFullYear();
												meizzTheMonth = new Date().getMonth()+1;
												today=new Date().getDate();
												if(meizzTheMonth<10){meizzTheMonth="0"+meizzTheMonth;}   
												if(today<10){today="0"+today;}
												outObject=meizzTheYear+""+meizzTheMonth+""+today; 
												dcmainPanel.loadFame('/msa/main.xp?Fun=ltmsaformExport+gid=-1+uid=-1+url=+dip=+date='+outObject+'+btype=unit1+bbyte=+fbtype=unit1++fbbyte=+osort=nosort+orderby=0+matype=-1',"dcmaintabid1",n.text);
                   }
                   else{
                   	   var meizzTheYear,meizzTheMonth,today,outObject;
											meizzTheYear = new Date().getFullYear();
											meizzTheMonth = new Date().getMonth()+1;
											today=new Date().getDate();
											if(meizzTheMonth<10){meizzTheMonth="0"+meizzTheMonth;}   
											if(today<10){today="0"+today;}
											outObject=meizzTheYear+""+meizzTheMonth+""+today;  
											dcmainPanel.loadFame('/msa/main.xp?Fun=ltwblogshowwebquery+groupid=-1+uid=-1+flag='+n.id+'+host=+sdate='+outObject+'+osort=nosort+orderby=0',"dcmaintabid1",n.text);
                   }
					    	}
		 });

     
    var dcgid,dcglevel,dcuid;
	  dcgid=0;
	  dcglevel=0;
    var groupPanel = new Ext.tree.TreePanel({
        region:'north',
        tbar:[new Ext.form.TextField({ width: 200,
		     					emptyText:'Find a Group',
							    listeners:{
							       render: function(f){
							         f.el.on('keydown', filterTree, f, {buffer: 350});
		        				 }
		              }
		    })],
        border:true,
        margins:'0 5 0 0',
        height: (screen.height-300)/2,
        useArrows:true,
        autoScroll:true,
        animate:true,
        rootVisible: false,
        title:'部门列表',
				collapsible:true,
        root: {
            nodeType: 'async'
        },  
        loader: new Ext.tree.TreeLoader({
						dataUrl:'/msa/main.xp?Fun=msaGroupTreeView',
						baseParams:{Fun:'msaGroupTreeView'} 
				}),
				listeners: {
				    click: function(node, e){
				      dcgid=node.id;
				      dcglevel=node.getDepth()-1;
			        if(dcgid=="-1" || dcgid==""){
									return;
							}else{	
								  //peoplePanel.getLoader().load(peoplePanel.root);
								  peoplestore.baseParams={Fun:'msaUserTree',gid:dcgid} 
						      peoplestore.load();
								  var myurl="/msa/main.xp?Fun=msaShowPeopleRecord&gid=" + node.id + "&uid=-1";
								  //parent.mainviewfrm.location.href=myurl;
								  dcmainPanel.loadFame(myurl,"dcmaintabid1",node.text );
				      }
				    }
			  }	
    });
    
     var filter = new Ext.tree.TreeFilter(groupPanel, {
			   clearBlank: true,
			   autoClear: true
	  });
	  
    var hiddenPkgs = [];
	  function filterTree(e){
	   			var text = e.target.value;
	        Ext.each(hiddenPkgs, function(n){
	             n.ui.show();
	        });

	         if(!text){
	             filter.clear();
	             return;
	         }
	         groupPanel.expandAll();
	         var re = new RegExp(Ext.escapeRe(text), 'i');
	         filter.filterBy(function(n){
	         		var textval = n.text;
	            return !n.isLeaf() || re.test(n.text);
	         });

	         hiddenPkgs = [];
	         groupPanel.root.cascade(function(n) {
	             if(!n.isLeaf()&& n.ui.ctNode.offsetHeight<3){
	                 n.ui.hide();
	                 hiddenPkgs.push(n);
	             }
	         });

	  }

	 
	  
    var peoplestore = new Ext.data.JsonStore({
						    root: 'Results',
					        totalProperty: 'totalCount',
					        url:'/msa/main.xp',
					        baseParams:{Fun:'msaUserTree',gid:dcgid},
						      fields: [
						         {name: 'id'},
						       	 {name: 'qtip'},
						         {name: 'text'}
						    ]
		});
		peoplestore.load();
		var peoplebbar = new Ext.PagingToolbar({   
	      pageSize: 50,   
	      store: peoplestore
	  }); 
		var peoplegrid = new Ext.grid.GridPanel({
			    tbar:[
						{xtype: "panel", border: false,
	            items: [new Ext.form.TwinTriggerField({
	                xtype: 'twintriggerfield', emptyText: "查找用户", name:"searchname", width: 200,
	                trigger1Class: 'x-form-clear-trigger',
	                trigger2Class: 'x-form-search-trigger',
	                onTrigger1Click: function(f) {
	                    this.reset();
	                },
	                onTrigger2Click: function(f) {
							   			peoplestore.baseParams={Fun:'msaUserTree',gid:dcgid,searchname:searchname.value}
										  peoplestore.load();
	                }
	            })]
        		}
		      ],
	        store: peoplestore,
	        viewConfig:{forceFit: true, scrollOffset: 0},
	        columns: [
	            {id:'id',header: "用户名", width: 200, sortable: true, dataIndex: 'text'}
	        ],
	        stripeRows: true,
	        region:'center',
	        bbar:peoplebbar, 
	        height: (screen.height-300)/2,
	        width: 280,
	        frame:true,
			    collapsible:false,
			    loadMask:{msg:'正在加载数据，请稍侯……'},
			    draggable:false
	   }); 
	   
	   peoplegrid.addListener('rowclick', rightClickFn);
	   function rightClickFn(client, rowIndex, e) { 
						 detailEl= rowIndex;
						 var record = peoplegrid.getStore().getAt(detailEl);
						 dcuid=record.get("id");     
					   var myurl="/msa/main.xp?Fun=msaShowPeopleRecord&gid="+dcgid + "&uid="+dcuid;
						 dcmainPanel.loadFame(myurl,"dcmaintabid1",record.get("text") );
					   
		 }  
    
    
//		var dcploader=new Ext.tree.TreeLoader({
//		    			dataUrl:'/msa/main.xp',
//							baseParams:{Fun:'msaUserTree'}
//		});
//    dcploader.on("beforeload", function(loader, node) {
//            loader.baseParams.gid = dcgid;
//            loader.baseParams.level = dcglevel;
//    });
//    var peoplePanel = new Ext.tree.TreePanel({
//        region:'center',
//        border:true,
//        margins:'0 5 0 0',
//        title:'人员列表',
//				collapsible:true,
//        useArrows:true,
//        autoScroll:true,
//        animate:true,
//        rootVisible: false,
//        root: {
//            nodeType: 'async'
//        },
//        loader:dcploader,
//				listeners: {
//				    click: function(node, e){
//				      dcuid=node.id;
//			        if(dcuid=="-1" || dcuid==""){
//									return;
//							}else{
//								var myurl="/msa/main.xp?Fun=msaShowPeopleRecord&gid="+dcgid + "&uid="+node.id;
//								//parent.mainviewfrm.location.href=myurl;
//								dcmainPanel.loadFame(myurl,"dcmaintabid1",node.text );
//				      }
//				    }
//			  }	
//    });
     
		 
		 var dcpeoplePanel = new Ext.Panel({
				    title:'人员',
				    collapsible:false,
				    layout: 'border',
				    items: [groupPanel,peoplegrid]
		 });
     
		 
		 var datePanel = new Ext.tree.TreePanel({
		 	  title:'日期',
        useArrows:true,
        autoScroll:true,
        animate:true,
        rootVisible: false,
        root: {
            nodeType: 'async'
        },  
        loader: new Ext.tree.TreeLoader({
						dataUrl:'/msa/main.xp?Fun=msaDateTree',
						baseParams:{Fun:'msaDateTree'} 
				}),
				listeners: {
				     click: function(node, e){
								  var myurl="/msa/main.xp?Fun=msaShowTimeRecord&sdate=" + node.id;
								  //parent.mainviewfrm.location.href=myurl;
								  dcmainPanel.loadFame(myurl,"dcmaintabid1",node.text);
				     }
			  }	
    });
		 
				    
		 var datacentertab = new Ext.TabPanel({
		 	              id: 'dcwestpanel',
		 	              collapsible: true,
                    region:'west',
                    split:true,
                    width: 220,
                    minSize: 200,
                    maxSize: 400,
										border:false,
										activeTab:1,
										margins:'0 5 0 0',
                    tbar: [{
						            icon:'/msa/extjs/resources/images/default/tree/drop-add.gif',
						            iconCls:'drop-add',						            
						            text:'关键字检索',
						            handler: function()
												{
												 //datacentertab.items.get(1).show();
												  //document.getElementById("dcmainviewfrm").src='/msa/datacenter/searchIndex.htm';
												  dcmainPanel.loadFame('/msa/datacenter/searchIndex.htm',"dcmaintabid1","搜索中心");
								          //mainLoadHelpDoc('/msa/help/sysruletdflow.htm');
								        },
						            scope: this
						        }],
										items:[datePanel,dcdataTree,dcpeoplePanel]
		 });

     var dcfootBar = new Ext.ux.StatusBar({
				region : "south",
				margins: '2 2 2 2',
				items : ["->", "<b>MSA&reg 宝创科技版权所有</b>  "]
	   });
     
   