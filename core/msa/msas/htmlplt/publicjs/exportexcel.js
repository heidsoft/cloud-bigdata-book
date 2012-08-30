function downloadViewDataActive(grid)
{
		
			try {
				var xls = new ActiveXObject ("Excel.Application");
			}
			catch(e) {
				alert( "要打印该表，您必须安装Excel电子表格软件，同时浏览器须使用“ActiveX 控件”，您的浏览器须允许执行控件。 请点击【帮助】了解浏览器设置方法！");
				return "";
			}
			xls.visible =true; //设置excel为可见
			var xlBook = xls.Workbooks.Add;
			var xlSheet = xlBook.Worksheets(1);
			
			var cm = grid.getColumnModel();
			var colCount = cm.getColumnCount();
			var temp_obj = [];
			//只下载没有隐藏的列(isHidden()为true表示隐藏,其他都为显示)
			//临时数组,存放所有当前显示列的下标
			for(i=0;i<colCount;i++){
				if(cm.isHidden(i) == true){
				}else{
					temp_obj.push(i);
				}
			}
			for(i=1;i<=temp_obj.length;i++){
				//显示列的列标题
				xlSheet.Cells(1,i).Value = cm.getColumnHeader(temp_obj[i - 1]);
			}
			var store = grid.getStore();
			var recordCount = store.getCount();
			var view = grid.getView();
			for(i=1;i<=recordCount;i++){
				for(j=1;j<=temp_obj.length;j++){
					//EXCEL数据从第二行开始,故row = i + 1;
					xlSheet.Cells(i + 1,j).Value = view.getCell(i - 1,temp_obj[j - 1]).innerText;
				}
			}
			xlSheet.Columns.AutoFit;
			xls.ActiveWindow.Zoom = 75
			xls.UserControl = true; //很重要,不能省略,不然会出问题 意思是excel交由用户控制
		  xls=null;
		  xlBook=null;
		  xlSheet=null;
}

function downloadViewData(grid){
			var vExportContent = grid.getExcelXml(); 
			alert(vExportContent);
			if (Ext.isIE6 || Ext.isIE7 || Ext.isSafari || Ext.isSafari2 || Ext.isSafari3) {            
					Ext.Ajax.request({            
							url: '/msas/main.xp?Fun=msasDownExcel',             
							method: 'POST',                      
							callback: function(o, s, r) {                  
								//alert(r.responseText);               
							},          
							isUpload: true,     
							params: {exportContent: vExportContent}  
					}) 
			} else { 
				document.location = 'data:application/vnd.ms-excel;base64,' + Base64.encode(vExportContent);     
			} 
}

function downloadFile(url){  
      var elemIF = document.createElement("iframe");  
      elemIF.src = url;  
      elemIF.style.display = "none";  
      document.body.appendChild(elemIF);  
}
