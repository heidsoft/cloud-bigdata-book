		function dcloop(n){
				if(n<16){
					var panelname='dcgkpanel'+n;
					Ext.Ajax.request({
			          method : 'POST',
			          url:'/msa/main.xp',
			          success: function(response, options){
			          		var bd = Ext.getCmp(panelname).body;
								    //bd.update('').setStyle('background', '#fff');
								    bd.update(response.responseText);
				          	n++
				          	dcloop(n);
			          },
			          failure : function(){
			          },
			        	params:{Fun:'msaiplogCv',num:n}
					});
				}
				return -1;
		}
		
		Date.prototype.Format = function(fmt) 
		{
		    //author: meizz 
		    var o =
		    { 
		        "M+" : this.getMonth() + 1, //月份 
		        "d+" : this.getDate(), //日 
		        "h+" : this.getHours(), //小时 
		        "m+" : this.getMinutes(), //分 
		        "s+" : this.getSeconds(), //秒 
		        "q+" : Math.floor((this.getMonth() + 3) / 3), //季度 
		        "S" : this.getMilliseconds() //毫秒 
		    }; 
		    if (/(y+)/.test(fmt)) 
		        fmt = fmt.replace(RegExp.$1, (this.getFullYear() + "").substr(4 - RegExp.$1.length)); 
		    for (var k in o) 
		        if (new RegExp("(" + k + ")").test(fmt)) 
		            fmt = fmt.replace(RegExp.$1, (RegExp.$1.length == 1) ? (o[k]) : (("00" + o[k]).substr(("" + o[k]).length))); 
		    return fmt; 
		}
		Date.prototype.addDays = function(d)
		{
		    this.setDate(this.getDate() + d);
		};
		
    Ext.ux.Portal = Ext.extend(Ext.Panel, {
		    layout: 'column',
		    autoScroll:true,
		    cls:'x-portal',
		    defaultType: 'portalcolumn',
		    
		    initComponent : function(){
		        Ext.ux.Portal.superclass.initComponent.call(this);
		        this.addEvents({
		            validatedrop:true,
		            beforedragover:true,
		            dragover:true,
		            beforedrop:true,
		            drop:true
		        });
		    },
		
		    initEvents : function(){
		        Ext.ux.Portal.superclass.initEvents.call(this);
		        this.dd = new Ext.ux.Portal.DropZone(this, this.dropConfig);
		    },
		    
		    beforeDestroy: function() {
		        if(this.dd){
		            this.dd.unreg();
		        }
		        Ext.ux.Portal.superclass.beforeDestroy.call(this);
		    }
		});
		Ext.reg('portal', Ext.ux.Portal);


		Ext.ux.Portal.DropZone = function(portal, cfg){
		    this.portal = portal;
		    Ext.dd.ScrollManager.register(portal.body);
		    Ext.ux.Portal.DropZone.superclass.constructor.call(this, portal.bwrap.dom, cfg);
		    portal.body.ddScrollConfig = this.ddScrollConfig;
		};
		
		Ext.extend(Ext.ux.Portal.DropZone, Ext.dd.DropTarget, {
		    ddScrollConfig : {
		        vthresh: 50,
		        hthresh: -1,
		        animate: true,
		        increment: 200
		    },
		    unreg: function() {
		        //Ext.dd.ScrollManager.unregister(this.portal.body);
		        Ext.ux.Portal.DropZone.superclass.unreg.call(this);
		    }
		});

    Ext.ux.PortalColumn = Ext.extend(Ext.Container, {
	    layout: 'anchor',
	    autoEl: 'div',
	    defaultType: 'portlet',
	    cls:'x-portal-column'
			});
	  Ext.reg('portalcolumn', Ext.ux.PortalColumn);
  
  Ext.ux.Portlet = Ext.extend(Ext.Panel, {
    anchor: '100%',
    frame:true,
    collapsible:true,
    draggable:true,
    cls:'x-portlet'
	});
	Ext.reg('portlet', Ext.ux.Portlet);  
       
  var dcgktools = [
  {
        id:'refresh',
        handler: function(event, toolEl, p){
        	//p.getUpdater().update({url: "ExtAjax.jsp", scripts: true});
          var panelid=p.id.substring(9);
          //alert(p.id);
          Ext.Ajax.request({
	          method : 'POST',
	          url:'/msa/main.xp',
	          success: function(response, options){
	          		//alert(response.responseText);
	          		//var bd = Ext.getCmp('dcgkpanel'+panelid).body;
	          		var bd = p.body;
						    //bd.update('').setStyle('background', '#fff');
						    //bd.hide().update(response.responseText).slideIn('0', {stopFx:true,duration:.2});
						    bd.update(response.responseText);
	          },
	          failure : function(){
	          },
	        	params:{Fun:'msaiplogCv',num:panelid}
					});
        }
  }];
  var panelhight=225;
  var c0_r0 = new Ext.ux.Portlet({
  	            xtype:'portal',
						    id:'dcgkpanel0',
						    margins:'10 0 0 0',
								title: '数据汇总',
								draggable:false,
								layout:'fit',
								height:panelhight,
								collapsible : false,
								collapsed:false,
		            tools: dcgktools,
								items: []
    });
    var c0_r1 = new Ext.ux.Portlet({
					      id:'dcgkpanel4',
					      margins:'10 0 0 0',
								title: '数据汇总',
								draggable:false,
								layout:'fit',
								height:panelhight,
								collapsible : false,
								collapsed:false,
			          tools: dcgktools,
								items: []
    });

		var c0_r2=new Ext.ux.Portlet({
					      id:'dcgkpanel8',
					      margins:'10 0 0 0',
								title: '数据汇总',
								draggable:false,
								layout:'fit',
								height:panelhight,
								collapsible : false,
								collapsed:false,
							  tools: dcgktools,
							  hidden: true,
								items: []
       });
		var c0_r3=new Ext.ux.Portlet({
					      id:'dcgkpanel12',
					      margins:'10 0 0 0',
								title: '数据汇总',
								draggable:false,
								layout:'fit',
								height:panelhight,
								collapsible : false,
								collapsed:false,
								hidden: true,
							  tools: dcgktools,
								items: []
		});
   	var col_0 = new Ext.ux.PortalColumn({
		       			columnWidth:.24,
		            style:'padding:10px 0 10px 10px',
		            items:[c0_r0,c0_r1,c0_r2,c0_r3]
   	});
    var c1_r0 = new Ext.ux.Portlet({
				    id:'dcgkpanel1',
				    margins:'10 0 0 0',
						title: '数据汇总',
						draggable:false,
						layout:'fit',
						height:panelhight,
						collapsible : false,
						collapsed:false,
						tools: dcgktools,
						items: []
		});
		var c1_r1 = new Ext.ux.Portlet({
				    id:'dcgkpanel5',
				    margins:'10 0 0 0',
						title: '数据汇总',
						draggable:false,
						layout:'fit',
						height:panelhight,
						collapsible : false,
						collapsed:false,
		        tools: dcgktools,
						items: []
		});
		var c1_r2 = new Ext.ux.Portlet({
						id:'dcgkpanel9',
						margins:'10 0 0 0',
						title: '数据汇总',
						draggable:false,
						layout:'fit',
						height:panelhight,
						collapsible : false,
						collapsed:false,
						hidden: true,
		        tools: dcgktools,
						items: []
		});
		var c1_r3 = new Ext.ux.Portlet({
				    id:'dcgkpanel13',
				    margins:'10 0 0 0',
						title: '数据汇总',
						draggable:false,
						layout:'fit',
						height:panelhight,
						collapsible : false,
						collapsed:false,
						hidden: true,
		        tools: dcgktools,
						items: []
		});
		var col_1 = new Ext.ux.PortalColumn({
                columnWidth:.24,
                style:'padding:10px 0 10px 10px',
                items:[c1_r0,c1_r1,c1_r2,c1_r3]
		});
		var c2_r0 = new Ext.ux.Portlet({
				    id:'dcgkpanel2',
				    margins:'10 0 0 0',
						title: '数据汇总',
						draggable:false,
						layout:'fit',
						height:panelhight,
						collapsible : false,
						collapsed:false,
						tools: dcgktools,
						items: []
		});
		var c2_r1 = new Ext.ux.Portlet({
				      id:'dcgkpanel6',
				      margins:'10 0 0 0',
							title: '数据汇总',
							draggable:false,
						  layout:'fit',
							height:panelhight,
							collapsible : false,
							collapsed:false,
		          tools: dcgktools,
							items: []
		});
		var c2_r2 = new Ext.ux.Portlet({
				    id:'dcgkpanel10',
				    margins:'10 0 0 0',
						title: '数据汇总',
						draggable:false,
						layout:'fit',
						height:panelhight,
						collapsible : false,
		        tools: dcgktools,
		        hidden: true,
						collapsed:false,
						items: []
		});
		var c2_r3 = new Ext.ux.Portlet({
				    id:'dcgkpanel14',
				    margins:'10 0 0 0',
						title: '数据汇总',
						draggable:false,
						layout:'fit',
						height:panelhight,
						collapsible : false,
		        tools: dcgktools,
		        hidden: true,
						collapsed:false,
						items: []
		});
		var col_2 = new Ext.ux.PortalColumn({
              columnWidth:.24,
              style:'padding:10px 0 10px 10px',
              items:[c2_r0,c2_r1,c2_r2,c2_r3]
		});
		var c3_r0 = new Ext.ux.Portlet({
				      id:'dcgkpanel3',
				      margins:'10 0 0 0',
							title: '数据汇总',
							draggable:false,
							layout:'fit',
							height:panelhight,
							collapsible : false,
							collapsed:false,
							tools: dcgktools,
							items: []
		});
		var c3_r1 = new Ext.ux.Portlet({
				      id:'dcgkpanel7',
				      margins:'10 0 0 0',
							title: '数据汇总',
							draggable:false,
						  layout:'fit',
							height:panelhight,
							collapsible : false,
							collapsed:false,
		          tools: dcgktools,
							items: []
		});
		var c3_r2 = new Ext.ux.Portlet({
				     id:'dcgkpanel11',
				     margins:'10 0 0 0',
						 title: '数据汇总',
						 draggable:false,
						 layout:'fit',
						 height:panelhight,
						 collapsible : false,
						 collapsed:false,
						 hidden: true,
		         tools: dcgktools,
						 items: []
		});
		var c3_r3 = new Ext.ux.Portlet({
				     id:'dcgkpanel15',
				     margins:'10 0 0 0',
						 title: '数据汇总',
						 draggable:false,
						 layout:'fit',
						 height:panelhight,
		         tools: dcgktools,
						 collapsible : false,
						 collapsed:false,
						 hidden: true,
						 items: []
		});
		var col_3 = new Ext.ux.PortalColumn({
                columnWidth:.24,
                style:'padding:10px 0 10px 10px',
                items:[c3_r0,c3_r1,c3_r2,c3_r3]
		});
		_dcgkportal = new Ext.ux.Portal({
				xtype:'portal',
				border: false,
        //region:'center',
        //margins:'35 5 5 0',
        items:[col_0,col_1,col_2,col_3]
		});
		var dcgkmenu = new Ext.menu.Menu({
	    id: 'dcgkbasicMenu',
	    items: [new Ext.menu.Item(
	       {
	            text: '浏览4天内数据',
	            handler: function(){
								for(var k=0;k<16;k++){
									if(k>=4){
										var hidename='dcgkpanel'+k;
										var bd = Ext.getCmp(hidename);
										bd.hide();
									}
								}
						  }
	        }),
	        new Ext.menu.Item({
	            text: '浏览8天内数据',
	            handler: function() {
									for(var k=0;k<16;k++){
										var hidename='dcgkpanel'+k;
										if(k>=8){
											var bd = Ext.getCmp(hidename);
											bd.hide();
										}else{
											var bd = Ext.getCmp(hidename);
											bd.show();
										}
									}
							}
	        }),
	        new Ext.menu.Item({
	            text: '浏览12天内数据',
	            handler: function() {
									for(var k=0;k<16;k++){
										var hidename='dcgkpanel'+k;
										if(k>=12){
											var bd = Ext.getCmp(hidename);
											bd.hide();
										}else{
											var bd = Ext.getCmp(hidename);
											bd.show();
										}
									}
							}
	        }),
	        new Ext.menu.Item({
	            text: '浏览16天内数据',
	            handler: function() {
									for(var k=0;k<16;k++){
										var hidename='dcgkpanel'+k;
										var bd = Ext.getCmp(hidename);
										bd.show();
									}
							}
	        }),
	        '-'
	    ]
		});
			
		var dcgkmainpanel= new Ext.Panel({
				xtype:'portal',
				region: 'center',
    		height:screen.height,
        autoScroll:true,
        collapsible: false,
        split:true,
        layout:'fit',
        margins:'0 5 0 0',
        tbar:[{text:"显示选项",
        			pressed: true,
	            icon: '/msa/images/menu-show.gif',
	            enableToggle:true,
	            cls: 'x-btn-text-icon details',
        			menu:dcgkmenu}],
        items:[_dcgkportal]        
		});
		
    for(var j=0;j<16;j++){
			var myname='dcgkpanel'+j;
			var now = new Date();
			now.addDays(-j);//加减日期操作
			Ext.getCmp(myname).setTitle(now.Format("yyyy-MM-dd")); 
    }


