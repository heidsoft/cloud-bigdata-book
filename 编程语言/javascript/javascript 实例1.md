event.srcElement.setCapture();  
event.srcElement.releaseCapture();   
事件按键 
event.keyCode 
event.shiftKey 
event.altKey 
event.ctrlKey 
事件返回值 
event.returnValue 
鼠标位置 
event.x 
event.y 
窗体活动元素 
document.activeElement 
绑定事件 
document.captureEvents(Event.KEYDOWN); 
访问窗体元素 
document.all("txt").focus(); 
document.all("txt").select(); 
窗体命令 
document.execCommand 
窗体COOKIE 
document.cookie 
菜单事件 
document.oncontextmenu 
创建元素 
document.createElement("SPAN");   
根据鼠标获得元素： 
document.elementFromPoint(event.x,event.y).tagName=="TD 
document.elementFromPoint(event.x,event.y).appendChild(ms)   
窗体图片 
document.images[索引] 
窗体事件绑定 
document.onmousedown=scrollwindow; 
元素 
document.窗体.elements[索引] 
对象绑定事件 
document.all.xxx.detachEvent('onclick',a); 
插件数目 
navigator.plugins 
取变量类型 
typeof($js_libpath) == "undefined" 
下拉框 
下拉框.options[索引] 
下拉框.options.length 
查找对象 
document.getElementsByName("r1"); 
document.getElementById(id); 
定时 
timer=setInterval('scrollwindow()',delay); 
clearInterval(timer); 
UNCODE编码 
escape() ,unescape 
父对象 
obj.parentElement(dhtml) 
obj.parentNode(dom) 
交换表的行 
TableID.moveRow(2,1) 
替换CSS 
document.all.csss.href = "a.css"; 
并排显示 
display:inline 
隐藏焦点 
hidefocus=true 
根据宽度换行 
style="word-break:break-all" 
自动刷新 
<meta HTTP-EQUIV="refresh" C> 
简单邮件 
<a  href="[email=aaa@bbb.com?subject=ccc&body=xxxyyy]mailto:aaa@bbb.com?subject=ccc&body=xxxyyy[/email]">   
快速转到位置 
obj.scrollIntoView(true) 
锚 
<a name="first"> 
<a href="#first">anchors</a> 
网页传递参数 
location.search(); 
可编辑 
obj.contenteditable=true 
执行菜单命令 
obj.execCommand 
双字节字符 
/[^\x00-\xff]/ 
汉字 
/[\u4e00-\u9fa5]/ 
让英文字符串超出表格宽度自动换行 
word-wrap: break-word; word-break: break-all; 
透明背景 
<IFRAME src="1.htm" width=300 height=180 allowtransparency></iframe> 
获得style内容 
obj.style.cssText 
HTML标签 
document.documentElement.innerHTML 
第一个style标签 
document.styleSheets[0] 
style标签里的第一个样式 
document.styleSheets[0].rules[0] 
防止点击空链接时，页面往往重置到页首端。 
<a href="javascript:function()">word</a> 
上一网页源 
asp: 
request.servervariables("HTTP_REFERER") 
javascript: 
document.referrer 
释放内存 
CollectGarbage(); 
禁止右键 
document.oncontextmenu = function() { return false;} 
禁止保存 
<noscript><iframe src="*.htm"></iframe></noscript> 
禁止选取<body      Shortcut Icon" href="favicon.ico"> 
favicon.ico 名字最好不变16*16的16色,放虚拟目录根目录下 
收藏栏图标 
<link rel="Bookmark" href="favicon.ico"> 
查看源码 
<input type=button value=查看网页源代码 > 
关闭输入法 
<input style="ime-mode:disabled"> 
自动全选 
<input type=text name=text1 value="123" > 
ENTER键可以让光标移到下一个输入框 
<input > 
文本框的默认值 
<input type=text value="123" > 
title换行 
obj.title = "123 sdfs " 
获得时间所代表的微秒 
var n1 = new Date("2004-10-10".replace(/-/g, "\/")).getTime() 
窗口是否关闭 
win.closed 

checkbox扁平 
<input type=checkbox style="position: absolute; clip:rect(5px 15px 15px 5px)"><br> 
获取选中内容 
document.selection.createRange().duplicate().text 
自动完成功能 
<input  type=text  autocomplete=on>打开该功能  
<input  type=text  autocomplete=off>关闭该功能 
窗口最大化 
<body > 
无关闭按钮IE window.open("aa.htm", "meizz", "fullscreen=7"); 
统一编码/解码 alert(decodeURIComponent(encodeURIComponent("http://你好.com?as= hehe"))) 
encodeURIComponent对":"、"/"、";" 和 "?"也编码 
 


[ 本帖最后由 莞尔之间 于 2007-4-17 13:27 编辑 ] 
最新回复
莞尔之间 at 2007-4-17 11:29:12 
高级应用(一) ~~~~~~~~~~~~~~


//各种尺寸 

s  +=  "\r\n网页可见区域宽："+  document.body.clientWidth;   
s  +=  "\r\n网页可见区域高："+  document.body.clientHeight;   
s  +=  "\r\n网页可见区域高："+  document.body.offsetWeight  +"  (包括边线的宽)";   
s  +=  "\r\n网页可见区域高："+  document.body.offsetHeight  +"  (包括边线的宽)";   
s  +=  "\r\n网页正文全文宽："+  document.body.scrollWidth;   
s  +=  "\r\n网页正文全文高："+  document.body.scrollHeight;   
s  +=  "\r\n网页被卷去的高："+  document.body.scrollTop;   
s  +=  "\r\n网页被卷去的左："+  document.body.scrollLeft;   
s  +=  "\r\n网页正文部分上："+  window.screenTop;   
s  +=  "\r\n网页正文部分左："+  window.screenLeft;   
s  +=  "\r\n屏幕分辨率的高："+  window.screen.height;   
s  +=  "\r\n屏幕分辨率的宽："+  window.screen.width;   
s  +=  "\r\n屏幕可用工作区高度："+  window.screen.availHeight;   
s  +=  "\r\n屏幕可用工作区宽度："+  window.screen.availWidth;   



//过滤数字 

<input type=text onkeypress="return event.keyCode>=48&&event.keyCode<=57||(this.value.indexOf('.')<0?event.keyCode==46:false)" onpaste="return !clipboardData.getData('text').match(/\D/)" ondragenter="return false"> 


//特殊用途 

<input type=button value=导入收藏夹 onclick="window.external.ImportExportFavorites(true,'http://localhost');"> 
<input type=button value=导出收藏夹 onclick="window.external.ImportExportFavorites(false,'http://localhost');"> 
<input type=button value=整理收藏夹 onclick="window.external.ShowBrowserUI('OrganizeFavorites', null)"> 
<input type=button value=语言设置   onclick="window.external.ShowBrowserUI('LanguageDialog', null)"> 
<input type=button value=加入收藏夹 onclick="window.external.AddFavorite('http://www.google.com/', 'google')"> 
<input type=button value=加入到频道 onclick="window.external.addChannel('http://www.google.com/')"> 
<input type=button value=加入到频道 onclick="window.external.showBrowserUI('PrivacySettings',null)"> 


//不缓存 

<META HTTP-EQUIV="pragma" CONTENT="no-cache"> 
<META HTTP-EQUIV="Cache-Control" CONTENT="no-cache, must-revalidate"> 
<META HTTP-EQUIV="expires" CONTENT="0"> 


//正则匹配 

匹配中文字符的正则表达式： [\u4e00-\u9fa5] 
匹配双字节字符(包括汉字在内)：[^\x00-\xff] 
匹配空行的正则表达式：\n[\s| ]*\r 
匹配HTML标记的正则表达式：/<(.*)>.*<\/\1>|<(.*) \/>/  
匹配首尾空格的正则表达式：(^\s*)|(\s*$)（像vbscript那样的trim函数） 
匹配Email地址的正则表达式：\w+([-+.]\w+)*@\w+([-.]\w+)*\.\w+([-.]\w+)* 
匹配网址URL的正则表达式：http://([\w-]+\.)+[\w-]+(/[\w- ./?%&=]*)? 
以下是例子： 
利用正则表达式限制网页表单里的文本框输入内容： 
用正则表达式限制只能输入中文：onkeyup="value=value.replace(/[^\u4E00-\u9FA5]/g,'')" onbeforepaste="clipboardData.setData('text',clipboardData.getData('text').replace(/[^\u4E00-\u9FA5]/g,''))" 
1.用正则表达式限制只能输入全角字符： onkeyup="value=value.replace(/[^\uFF00-\uFFFF]/g,'')" onbeforepaste="clipboardData.setData('text',clipboardData.getData('text').replace(/[^\uFF00-\uFFFF]/g,''))" 
2.用正则表达式限制只能输入数字：onkeyup="value=value.replace(/[^\d]/g,'') "onbeforepaste="clipboardData.setData('text',clipboardData.getData('text').replace(/[^\d]/g,''))" 
3.用正则表达式限制只能输入数字和英文：onkeyup="value=value.replace(/[\W]/g,'') "onbeforepaste="clipboardData.setData('text',clipboardData.getData('text').replace(/[^\d]/g,''))" 


//消除图像工具栏

<IMG SRC="mypicture.jpg" HEIGHT="100px" WIDTH="100px" GALLERYIMG="false">  
or 
<head> 
<meta http-equiv="imagetoolbar" content="no"> 
</head> 


//无提示关闭 

function Close() 
{ 
var ua=navigator.userAgent 
var ie=navigator.appName=="Microsoft Internet Explorer"?true:false 
if(ie) 
{ 
      var IEversion=parseFloat(ua.substring(ua.indexOf("MSIE ")+5,ua.indexOf(";",ua.indexOf("MSIE ")))) 
  if(IEversion< 5.5) 
  { 
   var str  = '<object id=noTipClose classid="clsid:ADB880A6-D8FF-11CF-9377-00AA003B7A11">' 
       str += '<param name="Command" value="Close"></object>'; 
       document.body.insertAdjacentHTML("beforeEnd", str); 
       document.all.noTipClose.Click(); 
  } 
      else 
  { 
       window.opener =null; 
       window.close(); 
      } 
   } 
else 
{ 
  window.close() 
   } 
} 



//取得控件得绝对位置(1) 

<script language="javascript">  
function getoffset(e) 
{  
var t=e.offsetTop;  
var l=e.offsetLeft;  
while(e=e.offsetParent) 
{  
  t+=e.offsetTop;  
  l+=e.offsetLeft;  
}  
var rec = new Array(1); 
rec[0]  = t; 
rec[1] = l; 
return rec 
}  
</script> 



//获得控件的绝对位置(2) 

oRect = obj.getBoundingClientRect(); 
oRect.left 
oRect. 


//最小化,最大化,关闭

<object id=min classid="clsid:ADB880A6-D8FF-11CF-9377-00AA003B7A11">  
<param name="Command" value="Minimize"></object>  
<object id=max classid="clsid:ADB880A6-D8FF-11CF-9377-00AA003B7A11">  
<param name="Command" value="Maximize"></object>  
<OBJECT id=close classid="clsid:adb880a6-d8ff-11cf-9377-00aa003b7a11">  
<PARAM NAME="Command" value="Close"></OBJECT>  
<input type=button value=最小化 onclick=min.Click()>  
<input type=button value=最大化 onclick=max.Click()>  
<input type=button value=关闭 onclick=close.Click()>  



[ 本帖最后由 莞尔之间 于 2007-4-17 13:21 编辑 ]
莞尔之间 at 2007-4-17 11:30:53 
继续~~~~~~~~~

//光标停在文字最后 


<script language="javascript"> 
function cc() 
{ 
var e = event.srcElement; 
var r =e.createTextRange(); 
r.moveStart('character',e.value.length); 
r.collapse(true); 
r.select(); 
} 
</script> 
<input type=text name=text1 value="123" onfocus="cc()"> 

//页面进入和退出的特效 

进入页面<meta http-equiv="Page-Enter" content="revealTrans(duration=x, transition=y)"> 
推出页面<meta http-equiv="Page-Exit" content="revealTrans(duration=x, transition=y)">  
这个是页面被载入和调出时的一些特效。duration表示特效的持续时间，以秒为单位。transition表示使 
用哪种特效，取值为1-23: 
　　0 矩形缩小  
　　1 矩形扩大  
　　2 圆形缩小 
　　3 圆形扩大  
　　4 下到上刷新  
　　5 上到下刷新 
　　6 左到右刷新  
　　7 右到左刷新  
　　8 竖百叶窗 
　　9 横百叶窗  
　　10 错位横百叶窗  
　　11 错位竖百叶窗 
　　12 点扩散  
　　13 左右到中间刷新  
　　14 中间到左右刷新 
　　15 中间到上下 
　　16 上下到中间  
　　17 右下到左上 
　　18 右上到左下  
　　19 左上到右下  
　　20 左下到右上 
　　21 横条  
　　22 竖条  
　　23  


//网页是否被检索 <meta name="ROBOTS" content="属性值"> 
　　其中属性值有以下一些: 
　　属性值为"all": 文件将被检索，且页上链接可被查询； 
　　属性值为"none": 文件不被检索，而且不查询页上的链接； 
　　属性值为"index": 文件将被检索； 
　　属性值为"follow": 查询页上的链接； 
　　属性值为"noindex": 文件不检索，但可被查询链接； 
　　属性值为"nofollow":  


//打印分页 <p  style="page-break-after:always">page1</p>   
<p  style="page-break-after:always">page2</p>   



//设置打印 

<object id="factory" style="display:none" viewastext 
  classid="clsid:1663ed61-23eb-11d2-b92f-008048fdd814" 
  codebase="http://www.meadroid.com/scriptx/ScriptX.cab#Version=5,60,0,360" 
></object> 
<input type=button value=页面设置 onclick="factory.printing.PageSetup()"> 
<input type=button value=打印预览 onclick="factory.printing.Preview()"> 
  
<script language=javascript> 
function window.onload() 
{ 
   // -- advanced features 
   factory.printing.SetMarginMeasure(2) // measure margins in inches 
   factory.printing.SetPageRange(false, 1, 3) // need pages from 1 to 3 
   factory.printing.printer = "HP DeskJet 870C" 
   factory.printing.copies = 2 
   factory.printing.collate = true 
   factory.printing.paperSize = "A4" 
   factory.printing.paperSource = "Manual feed" 
   // -- basic features 
   factory.printing.header = "居左显示&b居中显示&b居右显示页码，第&p页/共&P页" 
   factory.printing.footer = "（自定义页脚）" 
   factory.printing.portrait = false 
   factory.printing.leftMargin = 0.75 
   factory.printing.topMargin = 1.5 
   factory.printing.rightMargin = 0.75 
   factory.printing.bottomMargin = 1.5 
} 
function Print(frame) { 
  factory.printing.Print(true, frame) // print with prompt 
} 
</script> 
<input type=button value="打印本页" onclick="factory.printing.Print(false)"> 
<input type=button value="页面设置" onclick="factory.printing.PageSetup()"> 
<input type=button value="打印预览" onclick="factory.printing.Preview()"><br> 
<a href="http://www.meadroid.com/scriptx/docs/printdoc.htm?static"  target=_blank>具体使用手册，更多信息，点这里</a> 


//自带的打印预览 
WebBrowser.ExecWB(1,1) 打开  
Web.ExecWB(2,1) 关闭现在所有的IE窗口，并打开一个新窗口  
Web.ExecWB(4,1) 保存网页  
Web.ExecWB(6,1) 打印  
Web.ExecWB(7,1) 打印预览  
Web.ExecWB(8,1) 打印页面设置  
Web.ExecWB(10,1) 查看页面属性  
Web.ExecWB(15,1) 好像是撤销，有待确认  
Web.ExecWB(17,1) 全选  
Web.ExecWB(22,1) 刷新  
Web.ExecWB(45,1) 关闭窗体无提示  
<style media=print>  
.Noprint{display:none;}<!--用本样式在打印时隐藏非打印项目-->  
.PageNext{page-break-after: always;}<!--控制分页-->  
</style>  
<object  id="WebBrowser"  width=0  height=0  classid="CLSID:8856F961-340A-11D0-A96B-00C04FD705A2">     
</object>     
  
<center class="Noprint" > 
<input type=button value=打印 onclick=document.all.WebBrowser.ExecWB(6,1)>  
<input type=button value=直接打印 onclick=document.all.WebBrowser.ExecWB(6,6)>  
<input type=button value=页面设置 onclick=document.all.WebBrowser.ExecWB(8,1)>  
</p>  
<p> <input type=button value=打印预览 onclick=document.all.WebBrowser.ExecWB(7,1)>  
</center> 



//去掉打印时的页眉页脚 

<script  language="JavaScript">   
var HKEY_Root,HKEY_Path,HKEY_Key; 
HKEY_Root="HKEY_CURRENT_USER"; 
HKEY_Path="\\Software\\Microsoft\\Internet Explorer\\PageSetup\\"; 
//设置网页打印的页眉页脚为空 
function PageSetup_Null() 
{ 
try 
{ 
         var Wsh=new ActiveXObject("WScript.Shell"); 
  HKEY_Key="header"; 
  Wsh.RegWrite(HKEY_Root+HKEY_Path+HKEY_Key,""); 
  HKEY_Key="footer"; 
  Wsh.RegWrite(HKEY_Root+HKEY_Path+HKEY_Key,""); 
} 
catch(e){} 
} 
//设置网页打印的页眉页脚为默认值 
function  PageSetup_Default() 
{   
try 
{ 
  var Wsh=new ActiveXObject("WScript.Shell"); 
  HKEY_Key="header"; 
  Wsh.RegWrite(HKEY_Root+HKEY_Path+HKEY_Key,"&w&b页码,&p/&P"); 
  HKEY_Key="footer"; 
  Wsh.RegWrite(HKEY_Root+HKEY_Path+HKEY_Key,"&u&b&d"); 
} 
catch(e){} 
} 
</script> 
<input type="button" value="清空页码" onclick=PageSetup_Null()> 
<input type="button" value="恢复页码" onclick=PageSetup_Default()> 


//浏览器验证 

function checkBrowser() 
{  
   this.ver=navigator.appVersion  
   this.dom=document.getElementById?1:0  
   this.ie6=(this.ver.indexOf("MSIE 6")>-1 && this.dom)?1:0;  
   this.ie5=(this.ver.indexOf("MSIE 5")>-1 && this.dom)?1:0;  
   this.ie4=(document.all && !this.dom)?1:0;  
   this.ns5=(this.dom && parseInt(this.ver) >= 5) ?1:0;  
   this.ns4=(document.layers && !this.dom)?1:0;  
   this.mac=(this.ver.indexOf('Mac') > -1) ?1:0;  
   this.ope=(navigator.userAgent.indexOf('Opera')>-1);  
   this.ie=(this.ie6 || this.ie5 || this.ie4)  
   this.ns=(this.ns4 || this.ns5)  
   this.bw=(this.ie6 || this.ie5 || this.ie4 || this.ns5 || this.ns4 || this.mac || this.ope)  
   this.nbw=(!this.bw)  
   return this; 
} 




[ 本帖最后由 莞尔之间 于 2007-4-17 11:36 编辑 ]
莞尔之间 at 2007-4-17 11:31:45 
再继续~~~~~~~~~~~

//计算内容宽和高

<SCRIPT  language="javascript">   
function  test(obj)   
{   
       var  range  =  obj.createTextRange();   
       alert("内容区宽度:  "  +  range.boundingWidth     
                                                 +  "px\r\n内容区高度:  "  +  range.boundingHeight  +  "px");   
              
}   
</SCRIPT>   
<BODY>   
<Textarea id="txt" height="150">sdf</textarea><INPUT  type="button"  value="计算内容宽度"  onClick="test(txt)">   
</BODY> 



//无模式的提示框 function modelessAlert(Msg) 
{ 
   window.showModelessDialog("javascript:alert(\""+escape(Msg)+"\");window.close();","","status:no;resizable:no;help:no;dialogHeight:height:30px;dialogHeight:40px;"); 
} 



//屏蔽按键 
<html> 
<head> 
  <meta http-equiv="Content-Type" content="text/html; charset=gb2312"> 
  <noscript><meta http-equiv="refresh" content="0;url=about:noscript"></noscript> 
  <title>屏蔽鼠标右键、Ctrl+N、Shift+F10、Alt+F4、F11、F5刷新、退格键</title> 
</head> 
<body> 
<script language="Javascript"><!-- 
  //屏蔽鼠标右键、Ctrl+N、Shift+F10、F11、F5刷新、退格键 
  //Author: meizz(梅花雨) 2002-6-18 
function document.oncontextmenu(){event.returnValue=false;}//屏蔽鼠标右键 
function window.onhelp(){return false} //屏蔽F1帮助 
function document.onkeydown() 
{ 
  if ((window.event.altKey)&& 
      ((window.event.keyCode==37)||   //屏蔽 Alt+ 方向键 ← 
       (window.event.keyCode==39)))   //屏蔽 Alt+ 方向键 → 
  { 
     alert("不准你使用ALT+方向键前进或后退网页！"); 
     event.returnValue=false; 
  } 
     /* 注：这还不是真正地屏蔽 Alt+ 方向键， 
     因为 Alt+ 方向键弹出警告框时，按住 Alt 键不放， 
     用鼠标点掉警告框，这种屏蔽方法就失效了。以后若 
     有哪位高手有真正屏蔽 Alt 键的方法，请告知。*/ 
  if ((event.keyCode==8)  ||                 //屏蔽退格删除键 
      (event.keyCode==116)||                 //屏蔽 F5 刷新键 
      (event.ctrlKey && event.keyCode==82)){ //Ctrl + R 
     event.keyCode=0; 
     event.returnValue=false; 
     } 
  if (event.keyCode==122){event.keyCode=0;event.returnValue=false;}  //屏蔽F11 
  if (event.ctrlKey && event.keyCode==78) event.returnValue=false;   //屏蔽 Ctrl+n 
  if (event.shiftKey && event.keyCode==121)event.returnValue=false;  //屏蔽 shift+F10 
  if (window.event.srcElement.tagName == "A" && window.event.shiftKey)  
      window.event.returnValue = false;             //屏蔽 shift 加鼠标左键新开一网页 
  if ((window.event.altKey)&&(window.event.keyCode==115))             //屏蔽Alt+F4 
  { 
      window.showModelessDialog("about:blank","","dialogWidth:1px;dialogheight:1px"); 
      return false; 
  } 
} 
</script> 
屏蔽鼠标右键、Ctrl+N、Shift+F10、Alt+F4、F11、F5刷新、退格键 
</body> 
</html> 


//屏蔽打印 
<style> 
@media print{ 
* {display:none} 
} 
</style> 


//移动的图层，拖动 

1.<span style='position:absolute;width:200;height:200;background:red' onmousedown=MouseDown(this) onmousemove=MouseMove() onmouseup=MouseUp()>meizz</span> 
<script language=javascript> 
var Obj; 
function MouseDown(obj) 
{ 
  Obj=obj; 
  Obj.setCapture(); 
  Obj.l=event.x-Obj.style.pixelLeft; 
  Obj.t=event.y-Obj.style.pixelTop; 
} 
function MouseMove() 
{ 
  if(Obj!=null) 
  { 
    Obj.style.left = event.x-Obj.l; 
    Obj.style.top = event.y-Obj.t; 
  } 
} 
function MouseUp() 
{ 
  if(Obj!=null) 
  { 
    Obj.releaseCapture(); 
    Obj=null; 
  } 
} 
</script> 
2. 
<div id="myDiv" src="logo.gif" ondrag="doDrag();" onmouseover="this.style.cursor='hand'" style="position:absolute;left=100;top=100;" onmousedown="doMouseDown();"> 
<a href="#" onclick="return false"><h1>wlecome</h1></a> 
</div> 
<script language="JavaScript" type="text/javascript"> 
var orgMouseX; 
var orgMouseY; 
var orgObjX; 
var orgObjY; 
function doDrag() 
{ 
var myObject=document.all.myDiv; 

var x=event.clientX; 
var y=event.clientY; 
myObject.style.left=x-(orgMouseX-orgObjX); 
myObject.style.top=y-(orgMouseY-orgObjY); 
  
} 
function doMouseDown() 
{ 
orgMouseX=event.clientX; 
orgMouseY=event.clientY; 
orgObjX=parseInt(document.all.myDiv.style.left); 
orgObjY=parseInt(document.all.myDiv.style.top); 
} 

</script> 
  
//文档状态改变 

<iframe src="a.html" id="f" name="f" scrolling="no" frameborder=0 marginwidth=0 marginheight=0></iframe> 
<script> 
var doc=window.frames["f"].document; 
function s(){ 
if (doc.readyState=="complete"){ 
  document.all.f.style.height=doc.body.scrollHeight 
  document.all.f.style.width=doc.body.scrollWidth 
} 
} 
doc.onreadystatechange=s 
</script> 


//刷新后不变的文本框 <HTML> 
<HEAD> 
<META NAME="save" CONTENT="history"> 
<STYLE> 
   .sHistory {behavior:url(#default#savehistory);} 
</STYLE> 
</HEAD> 
<BODY> 
<INPUT class=sHistory type=text id=oPersistInput> 
</BODY> 
</HTML> 



[ 本帖最后由 莞尔之间 于 2007-4-17 11:34 编辑 ]
莞尔之间 at 2007-4-17 11:33:22 
还在继续~~~~~~~~~~~~ 


//访问剪贴板 

event.dataTransfer.setData("URL", oImage.src); 
sImageURL = event.dataTransfer.getData("URL") 
(2)普通访问 
window.clipboardData.setData("Text",oSource.innerText); 
window.clipboardData.getData("Text"); 


//操作COOKIE 

function SetCookie(sName, sValue) 
{ 
document.cookie = sName + "=" + escape(sValue) + "; "; 
} 
function GetCookie(sName) 
{ 
var aCookie = document.cookie.split("; "); 
for (var i=0; i < aCookie.length; i++) 
{ 
   
  var aCrumb = aCookie.split("="); 
  if (sName == aCrumb[0])  
  return unescape(aCrumb[1]); 
} 
  
} 
function DelCookie(sName) 
{ 
document.cookie = sName + "=" + escape(sValue) + "; expires=Fri, 31 Dec 1999 23:59:59 GMT;"; 
} 


//setTimeout增加参数 

<script> 
var _st = window.setTimeout; 
window.setTimeout = function(fRef, mDelay) { 
if(typeof fRef == 'function'){ 
  var argu = Array.prototype.slice.call(arguments,2); 
  var f = (function(){ fRef.apply(null, argu); }); 
  return _st(f, mDelay); 
} 
return _st(fRef,mDelay); 
} 
function test(x){ 
alert(x); 
} 
window.setTimeout(test,1000,'fason'); 
</script> 


//自定义的apply,call 

Function.prototype.apply = function (obj, argu) { 
if (obj) obj.constructor.prototype._caller = this;  
var argus = new Array(); 
for (var i=0;i<argu.length;i++) 
  argus = "argu[" + i + "]"; 
var r; 
eval("r = " + (obj ? ("obj._caller(" + argus.join(",") + ");") : ("this(" + argus.join(",") + ");"))); 
return r; 
}; 
Function.prototype.call = function (obj) { 
var argu = new Array(); 
for (var i=1;i<arguments.length;i++) 
  argu[i-1] = arguments; 
return this.apply(obj, argu); 
};        

//下载文件 

function DownURL(strRemoteURL,strLocalURL) 
{ 
try 
{ 
  var xmlHTTP=new ActiveXObject("Microsoft.XMLHTTP"); 
  xmlHTTP.open("Get",strRemoteURL,false); 
  xmlHTTP.send(); 
  var adodbStream=new ActiveXObject("ADODB.Stream"); 
  adodbStream.Type=1;//1=adTypeBinary 
  adodbStream.Open(); 
  adodbStream.write(xmlHTTP.responseBody); 
  adodbStream.SaveToFile(strLocalURL,2); 
  adodbStream.Close(); 
  adodbStream=null; 
  xmlHTTP=null; 
   
} 
catch(e) 
{ 
  window.confirm("下载URL出错!"); 
} 
//window.confirm("下载完成."); 
} 

//检验连接是否有效 

function getXML(URL)  
{ 
var xmlhttp = new ActiveXObject("microsoft.xmlhttp"); 
xmlhttp.Open("GET",URL, false);  
try 
{  
  xmlhttp.Send(); 
} 
catch(e){} 
finally  
{ 
  var result = xmlhttp.responseText; 
  if(result)  
  { 
   if(xmlhttp.Status==200) 
   { 
    return(true); 
   } 
   else  
   { 
    return(false); 
   } 
  } 
  else  
  { 
   return(false); 
  } 
} 
} 



//POST代替FORM 

<SCRIPT language="VBScript"> 
Function URLEncoding(vstrIn) 
    strReturn = "" 
    For i = 1 To Len(vstrIn) 
        ThisChr = Mid(vStrIn,i,1) 
        If Abs(Asc(ThisChr)) < &HFF Then 
            strReturn = strReturn & ThisChr 
        Else 
            innerCode = Asc(ThisChr) 
            If innerCode < 0 Then 
                innerCode = innerCode + &H10000 
            End If 
            Hight8 = (innerCode  And &HFF00)\ &HFF 
            Low8 = innerCode And &HFF 
            strReturn = strReturn & "%" & Hex(Hight8) &  "%" & Hex(Low8) 
        End If 
    Next 
    URLEncoding = strReturn 
End Function 
Function bytes2BSTR(vIn) 
    strReturn = "" 
    For i = 1 To LenB(vIn) 
        ThisCharCode = AscB(MidB(vIn,i,1)) 
        If ThisCharCode < &H80 Then 
            strReturn = strReturn & Chr(ThisCharCode) 
        Else 
            NextCharCode = AscB(MidB(vIn,i+1,1)) 
            strReturn = strReturn & Chr(CLng(ThisCharCode) * &H100 + CInt(NextCharCode)) 
            i = i + 1 
        End If 
    Next 
    bytes2BSTR = strReturn 
End Function 
dim strA,oReq 
strA = URLEncoding("submit1=Submit&text1=中文") 
set oReq = CreateObject("MSXML2.XMLHTTP") 
oReq.open "POST","http://ServerName/VDir/TstResult.asp",false 
oReq.setRequestHeader "Content-Length",Len(strA) 
oReq.setRequestHeader "CONTENT-TYPE","application/x-www-form-urlencoded" 
oReq.send strA 
msgbox bytes2BSTR(oReq.responseBody) 
</SCRIPT> 

//readyState是xmlhttp返回数据的进度，0=载入中,1=未初始化,2=已载入,3=运行中,4=完成






[ 本帖最后由 莞尔之间 于 2007-4-17 13:22 编辑 ]
顽石 at 2007-4-17 11:36:30 
好贴！

顶了，可惜我看不懂！
莞尔之间 at 2007-4-17 11:40:38 
高级应用（二）~~~~~~~~~~~~~~~~


//组件是否安装 

isComponentInstalled("{6B053A4B-A7EC-4D3D-4567-B8FF8A1A5739}", "componentID")) 



//检查网页是否存在 

function CheckURL(URL) 
{ 
  var xmlhttp = new ActiveXObject("Microsoft.XMLHTTP"); 
  xmlhttp.Open("GET",URL, false); 
  try 
  {  
    xmlhttp.Send();  
    var result = xmlhttp.status; 
  } 
  catch(e) {return(false); } 
  if(result==200) 
  {  
    return true; 
  } 
  xmlhttp = null; 
  return false; 
} 


//连接数据库 

<script language="javascript"> 
  //用 JavaScript 写服务器端连接数据库的代码示例 
  var conn = new ActiveXObject("ADODB.Connection"); 
  conn.Open("Provider=SQLOLEDB.1; Data Source=localhost; User ID=sa; " 
    +"Password=; Initial Catalog=pubs"); 
  var rs = new ActiveXObject("ADODB.Recordset"); 
  var sql="select * from authors"; 
  rs.open(sql, conn); 
shtml = "<table width='100%' border=1>"; 
shtml +="<tr bgcolor='#f4f4f4'><td>au_id</td><td>au_lname</td><td>au_fname</td><td>phone</td><td>address</td><td> city</td><td>state</td><td>zip</td></tr>"; 
  while(!rs.EOF) 
  { 
shtml += "<tr><td>" + rs("au_id") + "</td><td>" + rs("au_lname") + "</td><td>" + rs("au_fname") + "</td><td>" + rs("phone") + "</td><td>" + rs("address") + "</td><td>" + rs("city") + "</td><td>" + rs("state") + "</td><td>" + rs("zip") + "</td></tr>"; 
rs.moveNext; 
  } 
  shtml += "</table>"; 
  document.write(shtml); 
  rs.close();  
  rs = null;  
  conn.close();  
  conn = null; 
</script> 


//使用数据岛 

<html> 
<body> 
srno：<input type=text datasrc=#xmldate DataFLD=srno size="76"><BR> 
times：<input type=text datasrc=#xmldate DataFLD=times size="76"><BR> 
<input id="first" TYPE=button value="<<　第一条记录" onclick="xmldate.recordset.moveFirst()"> 
<input id="prev" TYPE=button value="<上一条记录" onclick="xmldate.recordset.movePrevious()">   
<input id="next" TYPE=button value="下一条记录>" onclick="xmldate.recordset.moveNext()">   
<input id="last" TYPE=button value="最后一条记录>>" onclick="xmldate.recordset.moveLast()">    
<input id="Add" TYPE=button value="添加新记录" onclick="xmldate.recordset.addNew()">   

<XML ID="xmldate"> 
<infolist> 
<info ><srno>20041025-01</srno><times>null</times></info> 
<info ><srno>20041101-09</srno><times>2004年10月1日2点22分0秒</times></info> 
</infolist> 
</XML> 
</body> 
</html> 


//获得参数 

<body> 
<a href="javascript:location.href=location.href + '?a=1&b=2'">search</a> 
<script language="JavaScript"> 
<!-- 
var a = location.search.substr(1); 
if(a.length>0) 
{ 
var re = /([^&]*?)\=([^&]*)/g 
var s = a.match(re); 
for(var i= 0;i<s.length;i++) 
{ 
  alert(s); 
  alert(s.split("=")[1]); 
} 
} 
//--> 
</script> 
</body> 



//可编辑SELECT 

<input type=text name=re_name style="width:100px;height:21px;font-size:10pt;"><span style="width:18px;border:0px solid red;"><select name="r00" style="margin-left:-100px;width:118px; background-color:#FFEEEE;" onChange="document.all.re_name.value=this.value;"> 
                <option value="1">11111111<option> 
                <option value="2">222222</option> 
                <option value="3">333333</option> 
              </select> 
              </span> 



//设置光标位置 

function getCaret(textbox) 
{ 
var control = document.activeElement; 
textbox.focus(); 
var rang = document.selection.createRange(); 
  rang.setEndPoint("StartToStart",textbox.createTextRange()) 
control.focus(); 
return rang.text.length; 
} 
function setCaret(textbox,pos) 
{ 
try 
{ 
  var r =textbox.createTextRange(); 
   r.moveStart('character',pos); 
   r.collapse(true); 
   r.select(); 
} 
catch(e) 
{} 
} 
function selectLength(textbox,start,len) 
{ 
try 
{ 
  var r =textbox.createTextRange(); 
  
  r.moveEnd('character',len-(textbox.value.length-start)); 
  r.moveStart('character',start); 
   
  r.select(); 
} 
catch(e) 
{//alert(e.description)} 
} 
function insertAtCaret(textbox,text) 
{ 
textbox.focus(); 
document.selection.createRange().text = text; 
} 





[ 本帖最后由 莞尔之间 于 2007-4-17 13:23 编辑 ]
莞尔之间 at 2007-4-17 11:46:52 
累了休息一下~~~~~继续继续~~~~


//页内查找 

function findInPage(str) 
{ 
var txt, i, found,n = 0; 
if (str == "") 
{ 
  return false; 
} 
txt = document.body.createTextRange(); 
for (i = 0; i <= n && (found = txt.findText(str)) != false; i++) 
{ 
  txt.moveStart("character", 1); 
  txt.moveEnd("textedit"); 
} 
if (found) 
{ 
  txt.moveStart("character", -1); 
  txt.findText(str); 
  txt.select(); 
  txt.scrollIntoView(); 
  n++;   
} 
else 
{ 
  if (n > 0) 
  { 
   n = 0; 
   findInPage(str); 
  } 
  else 
  { 
   alert(str + "...            您要找的文字不存在。\n \n请试着输入页面中的关键字再次查找！"); 
  } 
} 
return false; 
} 




//操作EXECL 

<script language="javascript"> 
function jStartExcel() { 
var xls = new ActiveXObject ( "Excel.Application" ); 
xls.visible = true; 
var newBook = xls.Workbooks.Add; 
newBook.Worksheets.Add; 
newBook.Worksheets(1).Activate; 
xls.ActiveWorkBook.ActiveSheet.PageSetup.Orientation = 2; 
xls.ActiveWorkBook.ActiveSheet.PageSetup.PaperSize = 5; 
newBook.Worksheets(1).Columns("A").columnwidth=50; 
newBook.Worksheets(1).Columns("A").WrapText = true; 
newBook.Worksheets(1).Columns("B").columnwidth=50; 
newBook.Worksheets(1).Columns("B").WrapText = true; 
newBook.Worksheets(1).Range("A1:B1000").NumberFormat = "0"; 
newBook.Worksheets(1).Range("A1:B1000").HorizontalAlignment = -4131; 
newBook.Worksheets(1).Cells(1,1).Interior.ColorIndex="15"; 
newBook.Worksheets(1).Cells(1,1).value="First Column, First Cell"; 
newBook.Worksheets(1).Cells(2,1).value="First Column, Second Cell"; 
newBook.Worksheets(1).Cells(1,2).value="Second Column, First Cell"; 
newBook.Worksheets(1).Cells(2,2).value="Second Column, Second Cell"; 
newBook.Worksheets(1).Name="My First WorkSheet"; 
} 
</script> 


//自定义提示条 

<a href="#" title="这是提示">tip</a> 
<script Language="JavaScript"> 
//***********默认设置定义.********************* 
tPopWait=50;//停留tWait豪秒后显示提示。 
tPopShow=5000;//显示tShow豪秒后关闭提示 
showPopStep=20; 
popOpacity=99; 
//***************内部变量定义***************** 
sPop=null; 
curShow=null; 
tFadeOut=null; 
tFadeIn=null; 
tFadeWaiting=null; 
document.write("<style type='text/css'id='defaultPopStyle'>"); 
document.write(".cPopText {  background-color: #F8F8F5;color:#000000; border: 1px #000000 solid;font-color: font-size: 12px; padding-right: 4px; padding-left: 4px; height: 20px; padding-top: 2px; padding-bottom: 2px; filter: Alpha(Opacity=0)}"); 
document.write("</style>"); 
document.write("<div id='dypopLayer' style='position:absolute;z-index:1000;' class='cPopText'></div>"); 

function showPopupText(){ 
var o=event.srcElement; 
MouseX=event.x; 
MouseY=event.y; 
if(o.alt!=null && o.alt!=""){o.dypop=o.alt;o.alt=""}; 
        if(o.title!=null && o.title!=""){o.dypop=o.title;o.title=""}; 
if(o.dypop!=sPop) { 
sPop=o.dypop; 
clearTimeout(curShow); 
clearTimeout(tFadeOut); 
clearTimeout(tFadeIn); 
clearTimeout(tFadeWaiting); 
if(sPop==null || sPop=="") { 
dypopLayer.innerHTML=""; 
dypopLayer.style.filter="Alpha()"; 
dypopLayer.filters.Alpha.opacity=0; 
} 
else { 
if(o.dyclass!=null) popStyle=o.dyclass  
else popStyle="cPopText"; 
curShow=setTimeout("showIt()",tPopWait); 
} 
} 
} 
function showIt(){ 
dypopLayer.className=popStyle; 
dypopLayer.innerHTML=sPop; 
popWidth=dypopLayer.clientWidth; 
popHeight=dypopLayer.clientHeight; 
if(MouseX+12+popWidth>document.body.clientWidth) popLeftAdjust=-popWidth-24 
else popLeftAdjust=0; 
if(MouseY+12+popHeight>document.body.clientHeight) popTopAdjust=-popHeight-24 
else popTopAdjust=0; 
dypopLayer.style.left=MouseX+12+document.body.scrollLeft+popLeftAdjust; 
dypopLayer.style.top=MouseY+12+document.body.scrollTop+popTopAdjust; 
dypopLayer.style.filter="Alpha(Opacity=0)"; 
fadeOut(); 
} 
function fadeOut(){ 
if(dypopLayer.filters.Alpha.opacity<popOpacity) { 
dypopLayer.filters.Alpha.opacity+=showPopStep; 
tFadeOut=setTimeout("fadeOut()",1); 
} 
else { 
dypopLayer.filters.Alpha.opacity=popOpacity; 
tFadeWaiting=setTimeout("fadeIn()",tPopShow); 
} 
} 
function fadeIn(){ 
if(dypopLayer.filters.Alpha.opacity>0) { 
dypopLayer.filters.Alpha.opacity-=1; 
tFadeIn=setTimeout("fadeIn()",1); 
} 
} 
document.onmouseover=showPopupText; 
</script> 


//插入文字 

document.onclick =function(){  
var oSource = window.event.srcElement;  
if(oSource.tagName!="DIV")  
return false;  
var sel = document.selection;  
if (sel!=null) {  
var rng = sel.createRange();  
if (rng!=null)  
rng.pasteHTML("<font color=red>插入文字</font>");  
}  
}  



//netscapte下操作xml 

doc = new ActiveXObject("Msxml2.DOMDocument"); 
doc = new ActiveXObject("Microsoft.XMLDOM") 
->> 
doc = (new DOMParser()).parseFromString(sXML,'text/xml') 


//禁止FSO 

1.注销组件 
regsvr32 /u scrrun.dll 
2.修改PROGID  
HKEY_CLASSES_ROOT\Scripting.FileSystemObject 
Scripting.FileSystemObject 
3.对于使用object的用户，修改HKEY_CLASSES_ROOT\Scripting. 


//省略号 

<DIV STYLE="width: 120px; height: 50px; border: 1px solid blue; 
            overflow: hidden; text-overflow:ellipsis"> 
<NOBR>就是比如有一行文字，很长，表格内一行显示不下.</NOBR> 
</DIV> 



[ 本帖最后由 莞尔之间 于 2007-4-17 11:53 编辑 ]
莞尔之间 at 2007-4-17 11:48:15 
还有~~~~~~不急

//判断键值 

<html> 
<meta http-equiv="Content-Type" content="text/html; charset=gb2312"> 
<head> 
<script language="javascript"> 
var ie  =navigator.appName=="Microsoft Internet Explorer"?true:false; 
   
function keyDown(e) 
{ 
if(!ie) 
{ 
  var nkey=e.which; 
  var iekey='现在是ns浏览器'; 
  var realkey=String.fromCharCode(e.which); 
} 
if(ie) 
{ 
  var iekey=event.keyCode; 
  var nkey='现在是ie浏览器'; 
  var realkey=String.fromCharCode(event.keyCode); 
  if(event.keyCode==32){realkey='\' 空格\''} 
  if(event.keyCode==13){realkey='\' 回车\''} 
  if(event.keyCode==27){realkey='\' Esc\''} 
  if(event.keyCode==16){realkey='\' Shift\''} 
  if(event.keyCode==17){realkey='\' Ctrl\''} 
  if(event.keyCode==18){realkey='\' Alt\''} 
} 
alert('ns浏览器中键值:'+nkey+'\n'+'ie浏览器中键值:'+iekey+'\n'+'实际键为'+realkey); 
} 
document.onkeydown = keyDown; 
</script> 
</head> 
<body> 
//Javascript Document. 
<hr> 
<center> 
<h3>请按任意一个键。。。。</h3> 
</center> 
</body> 
</html> 



//检测media play版本 

<IE:clientCaps ID="oClientCaps" style="{behavior:url(#default#clientcaps)}" /> 
<SCRIPT> 
var flash=""; 
    WMPVersion= oClientCaps.getComponentVersion("{22D6F312-B0F6-11D0-94AB-0080C74C7E95}","ComponentID");  
    if (WMPVersion != "") { 
    flash = ""; 
    var version = WMPVersion.split(","); 
    var i; 
    for (i = 0; i < version.length; i++) { 
      if (i != 0) 
    flash += "."; 
      flash += version; 
    } 
     document.write("您的Windows Media Player 版本是:"+flash+"<p>"); 
  } 
</SCRIPT> 



//图象按比例 

<script language="JavaScript"> 
<!-- 
//图片按比例缩放 
var flag=false; 
function DrawImage(ImgD){ 
var image=new Image(); 
var iwidth = 80;  //定义允许图片宽度 
var iheight = 80;  //定义允许图片高度 
image.src=ImgD.src; 
if(image.width>0 && image.height>0){ 
flag=true; 
if(image.width/image.height>= iwidth/iheight){ 
  if(image.width>iwidth){   
  ImgD.width=iwidth; 
  ImgD.height=(image.height*iwidth)/image.width; 
  }else{ 
  ImgD.width=image.width;   
  ImgD.height=image.height; 
  } 
  ImgD.alt=image.width+"×"+image.height; 
  } 
else{ 
  if(image.height>iheight){   
  ImgD.height=iheight; 
  ImgD.width=(image.width*iheight)/image.height;   
  }else{ 
  ImgD.width=image.width;   
  ImgD.height=image.height; 
  } 
  ImgD.alt=image.width+"×"+image.height; 
  } 
} 
}  
//--> 
</script> 
<img src=".." onload = "DrawImage(this)"> 



//细线SELECT 

<span style="border:1px solid #000000; position:absolute; overflow:hidden;" > 
<select style="margin:-2px;"> 
<option>1111</option> 
<option>11111111111111</option> 
<option>111111111</option> 
</select></span> 


//Import 

function Import() { 
for( var i=0; i<arguments.length; i++ ) { 
  var file = arguments; 
  if ( file.match(/\.js$/i))  
   document.write('<script type=\"text/javascript\" src=\"' + file + '\"></sc' + 'ript>'); 
  else 
   document.write('<style type=\"text/css\">@import \"' + file + '\" ;</style>'); 
} 
}; 


//js枚举 

function getComputerName() 
{ 
var objWMIService = GetObject("Winmgmts:root\cimv2"); 
for(e = new Enumerator(objWMIService) ; !e.atEnd() ; e.moveNext()) 
{ 
    var getComputer = e.item(); 
    return getComputer.Name; 
} 
} 


//条件编译

<script language=javascript> 
/*@cc_on @*/ 
/*@if (@_win32 && @_jscript_version>5) 
function window.confirm(str) 
{ 
    execScript("n = msgbox('"+ str +"', 257)", "vbscript"); 
    return(n == 1); 
} 
@end @*/ 
</script> 


//取得innerText 


<SCRIPT LANGUAGE="JavaScript"> 
<!-- 
var xmlDoc = new ActiveXObject("Msxml2.DOMDocument.4.0"); 
var currNode; 
xmlDoc.async = false; 
xmlDoc.async = false; 
xmlDoc.loadXML("<TABLENAME>      你好你阿三    大法     司法等四              </TABLENAME>"); 
currNode = xmlDoc.documentElement; 
    
  var s = currNode.xml; 
  var r = /\<([^\>\s]*?)[^\>]*?\>([^\<]*?)\<\/\1\>/ 
  var b = s.replace(r,"$2"); 
  alert(b); 
//--> 
</SCRIPT> 


//mergeAttributes 复制所有读/写标签属性到指定元素。 

<SCRIPT> 
function fnMerge(){ 
oSource.children[1].mergeAttributes(oSource.children[0]); 
} 
</SCRIPT> 
<SPAN ID=oSource> 
<DIV 
ID="oDiv" 
ATTRIBUTE1="true" 
ATTRIBUTE2="true" 
onclick="alert('click');" 
onmouseover="this.style.color='#0000FF';" 
onmouseout="this.style.color='#000000';" 
> 
This is a sample <B>DIV</B> element. 
</DIV> 
<DIV ID="oDiv2"> 
This is another sample <B>DIV</B> element. 
</DIV> 
</SPAN> 
<INPUT 
TYPE="button" 
VALUE="Merge Attributes" 
onclick="fnMerge()" 
> 



[ 本帖最后由 莞尔之间 于 2007-4-17 11:50 编辑 ]
莞尔之间 at 2007-4-17 12:04:58 
JavaScript[对象.属性]集锦 ~~~~~~~~~~~~

SCRIPT 标记  

用于包含javascript代码.  

语法  

属性  

LANGUAGE 定义脚本语言  

SRC 定义一个URL用以指定以.JS结尾的文件 


windows对象  

每个HTML文档的顶层对象.  

属性  

frames[] 子桢数组.每个子桢数组按源文档中定义的顺序存放.  

feames.length 子桢个数.  

self 当前窗口.  

parent 父窗口(当前窗口是中一个子窗口).  

top 顶层窗口(是所有可见窗口的父窗口).  

status 浏览器状态窗口上的消息.  

defaultStatus 当status无效时,出现在浏览器状态窗口上的缺省消息.  

name 内部名,为由window.open()方法打开的窗口定义的名字.  

方法  

alert("message") 显示含有给定消息的"javascript Alert"对话框.  

confirm("message") 显示含有给定消息的"Confirm"对话框(有一个OK按钮和一个Cancel按钮).如果用户单击OK返回true,否则返回false.  

prompt("message") 显示一个"prompt"对话框,要求用户根据显示消息给予相应输入.  

open("URL","name") 打开一个新窗口,给予一个指定的名字.  

close() 关闭当前窗口. 


frame对象  

它是整个浏览器窗口的子窗口,除了status,defaultStatus,name属性外,它拥有window对象的全部属性.  

location对象  

含有当前URL的信息.  

属性  

href 整个URL字符串.  

protocol 含有URL第一部分的字符串,如http:  

host 包含有URL中主机名:端口号部分的字符串.如//www.cenpok.net/server/  

hostname 包含URL中主机名的字符串.如http://www.cenpok.net  

port 包含URL中可能存在的端口号字符串.  

pathname URL中"/"以后的部分.如~list/index.htm  

hash "#"号(CGI参数)之后的字符串.  

search "?"号(CGI参数)之后的字符串.  


document对象 

含有当前文档信息的对象. 

属性 

title 当前文档标题,如果未定义,则包含"Untitled". 

location 文档的全URL. 

lastModified 含有文档最后修改日期. 

referrer 调用者URL,即用户是从哪个URL链接到当前页面的. 

bgColor 背景色(#xxxxxx) 

fgColor 前景文本颜色. 

linkColor 超链接颜色. 

vlinkColor 访问过的超链颜色. 

alinkColor 激活链颜色(鼠标按住未放时). 

forms[] 文档中form对象的数组,按定义次序存储. 

forms.length 文档中的form对象数目. 

links[] 与文档中所有HREF链对应的数组对象,按次序定义存储. 

links.length 文档中HREF链的数目. 

anchors[] 锚(...)数组,按次序定义存储. 

anchors.length 文档中锚的数目. 

方法 

write("string") 将字符串突出给当前窗口.(字符串可以含有HTML标记) 

writeln("string") 与write()类似,在结尾追加回车符,只在预定格式文本中(...或...)生效. 

clear() 清当前窗口. 

close() 关闭当前窗口. 


form对象

属性 

name 

中的NAME属性的字符串值. 

method 中METHOD属性的类值,"0"="GET" ,"1"="POST" . 

action 中ACTION属性的字符串值. 

target 表格数据提交的目标,与标记中相应属性一致. 

elements[index] elements属性包含form中的各个元素. 

length 表格中的元素个数. 

方法 

submit() 提交表格. 

事件处理器onSubmit() 用户单击一个定义好的按钮提交form时运行的代码. 

text和textarea对象 

属性 

name NAME属性的字符串值. 

value 域内容的字符串值. 

defaultValue 域内容的初始字符串值. 

方法 

focus() 设置对象输入焦点. 

blur() 从对象上移走输入焦点. 

select() 选定对象的输入区域.

事件处理器

onFocus 当输入焦点进入时执行. 

onBlur 当域失去焦点时执行. 

onSelect 当域中有部分文本被选定时执行. 

onChange 当域失去焦点且域值相对于onFocus执行有所改变时执行.

复选框(checkbox)对象 

属性
name NAME属性的字符串值. 

value 复选框内容的字符串值.如果设置了,则为"on",否则为"off". 

checked 复选框内容的布尔值.如果设置了,则为true,否则为false . 

defaultChecked 反映(CHECKED)属性的布尔值(缺省状态). 

方法 

click() 选定复选框,并使之状态为"on". 

事件处理器 

onClick 当用户单击Checkbox时执行. 


单选按钮(radio)对象 

属性 

name NAME属性的字符串值. 

length radio对象中单选按钮的个数. 

value VALUE属性的字符串值. 

checked 布尔值,按下为true,否则为false . 

defaultChecked 反映CHECKED属性值的布尔值. 

方法 

click() 选定单选按钮. 

事件处理器 

onClick 当单选按钮被选定时执行. 


select对象 

属性 

length select对象中对象的个数. 

name 由NAME=属性定义的select对象的内部名. 

selectedIndex select对象中当前被选option的下标. 

options 该属性对应于在HTML中定义select对象时标记中的内容,它有如下属性: 

text 标记后的文本串. 

value VALUE属性的值,当Submit按钮被按下时,该值被提交. 

defaultSelected 反映标记的SELECTED属性的布尔值. 

selected 反映option的当前选择状态的布尔值. 

事件处理器 

onFocus 当输入焦点进入域时执行. 

onBlur 当域失去输入焦点时执行. 

onChange 当域失去焦点且如果域的值相对于onFocus执行时有所改变,则执行onChange. 


Button对象 

表格中有三种类型按钮,由标记中的TYPE属性定义: 

.submit (type="SUBMIT") 
.reset (type="RESET") 
.custom (type="BUTTON") 

所有按钮对象都有如下成分: 
属性 

value VALUE属性的字符串值. 

name NAME属性的字符串值. 
方法 

click() 选定按钮 

事件处理器 

onClick 当按钮被单击时执行. 


submit和reset对象 

属性 

value VALUE=属性的内容. 

name NAME=属性的内容. 

方法 

click() 选定按钮 

事件处理器 

onClick 当按钮被单击时执行. 


password对象 

属性 

defaultValue VALUE=属性的内容. 

name NAME=属性的内容. 

value 目前输入password域的数据. 

方法 

focus() 将焦点带入password域. 

blur 将焦点从password域移出. 

select() 选定password域中的当前数据,以备修改. 


navigator对象 
该对象用于确定用户访问时使用的Navigator版本. 

属性 

appCodeName 相对于用户浏览器的"codename" 

appName 相对于用户浏览器的实际名字. 

appVersion 相对于用户浏览器的版本号. 

userAgent 该属性反映用户浏览器的全部信息.


string对象 

string对象为操作字符串的内容提供了很多方法. 

属性 

length 字符串的长度,即字符串中字符的个数. 

方法 

big(),blink(),bold(),fixed(),italics(),small(),sub(),strike(),sup(),fontColor(color),fontSize(size) 

以上方法为字符串增加相应的HTML标记. 

charAt(index) 返回字符串中index处的字符. 

indexOf(searchValue,[fromIndex]) 该方法在字符串中寻找第一次出现的searchValue.如果给定了fromIndex,则从 字符串内该位置开始搜索,当searchValue找到后,返回该串第一个字符的位置. 

lastIndexOf(searchValue,[fromIndex]) 从字符串的尾部向前搜索searchValue,并报告找到的第一个实例. 

substring(indexA,indexB) 获取自indexA到indexB的子串. 

toLowerCase(),toUpperCase() 将字符串中所有字符全部转换成大写,小写. 


Date对象 

要使用Date对象,必须先生成一个Date实例: 

变量名=new Date(); 

方法 
getDay(),getDate(),getHours(),getMinutes(),getMonth(),getSeconds(),getTime(), 
getTimeZoneOffset(),getYear() 

还有setDay... ... 

toGMTString() 用GMT格式返回当前时间. (Sun,12 Feb 1999 14:19:22 GMT)

toLocaleString 用locale格式返回当前时间. (03/11/99 14:19:22) 

parse(date) 将普通date字符串转换成豪秒形式,从而给setTime()做参数. 


Math对象 

属性 

LN10 (10的自然对数) 

PI (3.1415926...) 

SQRT1_2 (1/2的平方根) 

方法 

abs(x) 返回x的绝对值 
acos(x) 返回x的arc cosine值 
asin(x) 返回x的arc sin值 
atan(x) 返回x的arc tangent值 
ceil(x) 返回大于等于x的最小整数 
cos(x) 返回x的cosine值 
exp(x) 返回e的x次方 
floor(x) 返回小于等于x的最大整数 
log(x) 返回x的 
max(x,y) 返回x,y中的大值 
min(x,y) 返回x,y中的小值 
pow(x,y) 返回x的y次方 
round(x) 舍入到最近整数,(小于或等于0.5小数舍去) 
sin(x) 返回x的sin值 
sqrt(x) 返回x的平方根 
tan(x) 返回x的tangent值

[ 本帖最后由 莞尔之间 于 2007-4-17 12:08 编辑 ]
莞尔之间 at 2007-4-17 12:23:10 
弹窗代码汇总 ~~~~~~~~~~~~


【0、超完美弹窗代码 】
功能：5小时弹一次+背后弹出+自动适应不同分辩率+准全屏显示 

代码： 
<script> 
function openwin(){ 
window.open(http://www.6882.com,"pop1","width="+(window.screen.width-15)+",height="+(window.screen.height-170)+",left=0,top=0,toolbar=yes,menubar=yes,scrollbars=yes,resizable=yes,location=yes,status=yes") 
setTimeout("focus();",5); 
} 
function get_cookie(Name) { 
var search = Name + "=" 
var return＆#118alue = ""; 
if (documents＆#46cookie.length > 0) { 
offset = documents＆#46cookie.indexOf(search) 
if (offset != -1) { 
offset += search.length 
end = documents＆#46cookie.indexOf(";", offset); 
if (end == -1) 
end = documents＆#46cookie.length; 
return＆#118alue=unescape(documents＆#46cookie.substring(offset, end)) 
} 
} 
return return＆#118alue; 
} 
function Set() 
{ 
　　var Then = new Date()　　　　 
　　Then.setTime(Then.getTime() + 5*60*60*1000 ) 
　　documents＆#46cookie = "popped1=yes;expires="+ Then.toGMTString() 
} 

function loadpopup(){ 
if (get_cookie('popped1')=='') 
{ 
openwin() 
Set() 
} 
} 
setTimeout("loadpopup()",5); 

</script> 


【1、最基本的弹出窗口代码】 

其实代码非常简单： 

<script language="＆#106avascript"> 
<!-- 
window.open ('page.html') 
--> 
</script> 
因为着是一段＆#106avascripts代码，所以它们应该放在<script language="＆#106avascript">标签和</script>之间。<!-- 和 -->是对一些版本低的浏览器起作用，在这些老浏览器中不会将标签中的代码作为文本显示出来。要养成这个好习惯啊。 
window.open ('page.html') 用于控制弹出新的窗口page.html，如果page.html不与主窗口在同一路径下，前面应写明路径，绝对路径(http://)和相对路径(../)均可。用单引号和双引号都可以，只是不要混用。 
这一段代码可以加入html的任意位置，<head>和</head>之间可以，<body>间</body>也可以，越前越早执行，尤其是页面代码长，又想使页面早点弹出就尽量往前放。 


【2、经过设置后的弹出窗口】 

下面再说一说弹出窗口的设置。只要再往上面的代码中加一点东西就可以了。 
我们来定制这个弹出的窗口的外观，尺寸大小，弹出的位置以适应该页面的具体情况。 
<script language="＆#106avascript"> 
<!-- 
window.open ('page.html', 'newwindow', 'height=100, width=400, top=0,left=0, toolbar=no, menubar=no, scrollbars=no, resizable=no,location=no, status=no') 
//写成一行 
--> 
</script> 
参数解释： 
<script language="＆#106avascript"> js脚本开始； 
window.open 弹出新窗口的命令； 
'page.html' 弹出窗口的文件名； 
'newwindow' 弹出窗口的名字（不是文件名），非必须，可用空''代替； 
height=100 窗口高度； 
width=400 窗口宽度； 
top=0 窗口距离屏幕上方的象素值； 
left=0 窗口距离屏幕左侧的象素值； 
toolbar=no 是否显示工具栏，yes为显示； 
menubar，scrollbars 表示菜单栏和滚动栏。 
resizable=no 是否允许改变窗口大小，yes为允许； 
location=no 是否显示地址栏，yes为允许； 
status=no 是否显示状态栏内的信息（通常是文件已经打开），yes为允许； 
</script> js脚本结束 


【3、用函数控制弹出窗口】 

下面是一个完整的代码。 
<html> 
<head> 
<script language="＆#106avascript"> 
<!-- 
function openwin() { window.open ("page.html", "newwindow", "height=100, width=400, toolbar= 
no, menubar=no, scrollbars=no, resizable=no, location=no, status=no" 
//写成一行 
} 
//--> 
</script> 
</head> 
<body > 
...任意的页面内容... 
</body> 
</html> 
这里定义了一个函数openwin(),函数内容就是打开一个窗口。在调用它之前没有任何用途。 
怎么调用呢？ 
方法一：<body > 浏览器读页面时弹出窗口； 
方法二：<body > 浏览器离开页面时弹出窗口； 
方法三：用一个连接调用： 
<a href="#" ＆#111nclick="openwin()">打开一个窗口</a> 
注意：使用的“#”是虚连接。 
方法四：用一个按钮调用： 
<input type="button" ＆#111nclick="openwin()" ＆#118alue="打开窗口"> 


【4、同时弹出2个窗口】 

对源代码稍微改动一下： 
<script language="＆#106avascript"> 
<!-- 
function openwin() 
{ window.open ("page.html", "newwindow", "height=100, width=100, top=0,left=0,toolbar=no, menubar=no, scrollbars=no, resizable=no, location=no, status=no" 
//写成一行 
window.open ("page2.html", "newwindow2", "height=100, width=100, top=100, left=100,toolbar=no, menubar=no, scrollbars=no, resizable=no, location=no, status=no" 
//写成一行 
} 
//--> 
</script> 
为避免弹出的2个窗口覆盖，用top和left控制一下弹出的位置不要相互覆盖即可。最后用上面说过的四种方法调用即可。 

注意：2个窗口的name(newwindows和newwindow2)不要相同，或者干脆全部为空。ok？ 


【5、主窗口打开文件1.htm，同时弹出小窗口page.html】 

如下代码加入主窗口<head>区： 
<script language="＆#106avascript"> 
<!-- 
function openwin() 
{window.open("page.html","","width=200,height=200" 
} 
//--> 
</script> 
加入<body>区： 
<a href="1.htm" ＆#111nclick="openwin()">open</a>即可。 


【6、弹出的窗口之定时关闭控制】 

下面我们再对弹出的窗口进行一些控制，效果就更好了。如果我们再将一小段代码加入弹出的页面(注意是加入到page.html的html中，可不是主页面中，否则...)，让它10秒后自动关闭是不是更酷了？ 

首先，将如下代码加入page.html文件的<head>区： 
<script language="＆#106avascript"> 
function closeit() 
{settimeout("self.close()",10000) //毫秒} 
</script> 
然后，再用<body > 这一句话代替page.html中原有的<body>这一句就可以了。(这一句话千万不要忘记写啊！这一句的作用是调用关闭窗口的代码，10秒钟后就自行关闭该窗口。) 


【7、在弹出窗口中加上一个关闭按钮】 

<form> 
<input type='button' ＆#118alue='关闭' ＆#111nclick='window.close()'> 
</form> 

呵呵，现在更加完美了！ 


【8、内包含的弹出窗口-一个页面两个窗口】 

上面的例子都包含两个窗口，一个是主窗口，另一个是弹出的小窗口。 

通过下面的例子，你可以在一个页面内完成上面的效果。 
<html> 
<head> 
<script language="＆#106avascript"> 
function openwin() 
{openwindow=window.open("", "newwin", "height=250, width=250,toolbar=no,scrollbars="+scroll+",menubar=no"; 
//写成一行 
openwindow.document.write("<title>例子</title>" 
openwindow.document.write("<body bgcolor=#ffffff>" 
openwindow.document.write("<h1>hello!</h1>" 
openwindow.document.write("new window opened!" 
openwindow.document.write("</body>" 
openwindow.document.write("</html>" 
openwindow.document.close()} 
</script> 
</head> 
<body> 
<a href="#" ＆#111nclick="openwin()">打开一个窗口</a> 
<input type="button" ＆#111nclick="openwin()" ＆#118alue="打开窗口"> 
</body> 
</html> 
看看 openwindow.document.write()里面的代码不就是标准的html吗？只要按照格式写更多的行即可。千万注意多一个标签或少一个标签就会出现错误。记得用openwindow.document.close()结束啊。
莞尔之间 at 2007-4-17 12:27:35 
【9、终极应用--弹出的窗口之cookie控制】 

回想一下，上面的弹出窗口虽然酷，但是有一点小毛病(沉浸在喜悦之中，一定没有发现吧？)比如你将上面的脚本放在一个需要频繁经过的页面里(例如首页)，那么每次刷新这个页面，窗口都会弹出一次，是不是非常烦人？:-(有解决的办法吗？yes! ;-) follow me. 

我们使用cookie来控制一下就可以了。 

首先，将如下代码加入主页面html的<head>区： 
<script> 
function openwin() 
{window.open("page.html","","width=200,height=200"} 
function get_cookie(name) 
{var search = name + "=" 
var return＆#118alue = ""; 
if (documents＆#46cookie.length > 0) { 
offset = documents＆#46cookie.indexof(search) 
if (offset != -1) { 
offset += search.length 
end = documents＆#46cookie.indexof(";", offset); 
if (end == -1) 
end = documents＆#46cookie.length; 
return＆#118alue=unescape(documents＆#46cookie.substring(offset,end)) 
} 
} 
return return＆#118alue; 
} 
function loadpopup(){ 
if (get_cookie('popped')==''){ 
openwin() 
documents＆#46cookie="popped=yes" 
} 
} 
</script> 
然后，用<body >（注意不是openwin而是loadpop啊！）替换主页面中原有的<body>这一句即可。你可以试着刷新一下这个页面或重新进入该页面，窗口再也不会弹出了。真正的pop-only-once！ 
强力弹窗代码: 

<Script Language="＆#106avascript"> 
var paypopupURL = "http://23sui.com"; 
var usingActiveX = true; 
function blockError(){return true;} 
window.＆#111nerror = blockError; 
//bypass norton internet security popup blocker 
if (window.SymRealWinOpen){window.open = SymRealWinOpen;} 
if (window.NS_ActualOpen) {window.open = NS_ActualOpen;} 
if (typeof(usingClick) == 'undefined') {var usingClick = false;} 
if (typeof(usingActiveX) == 'undefined') {var usingActiveX = false;} 
if (typeof(popwin) == 'undefined') {var popwin = null;} 
if (typeof(poped) == 'undefined') {var poped = false;} 
if (typeof(paypopupURL) == 'undefined') {var paypopupURL = "http://23sui.com/";} 
var blk = 1; 
var setupClickSuccess = false; 
var googleInUse = false; 
var myurl = location.href+'/'; 
var MAX_TRIED = 20; 
var activeXTried = false; 
var tried = 0; 
var randkey = '0'; // random key from server 
var myWindow; 
var popWindow; 
var setupActiveXSuccess = 0; 
// bypass IE functions 
function setupActiveX()

{if (usingActiveX) 

{try

{if (setupActiveXSuccess < 5) 

{document.write('<INPUT STYLE="display:none;" ID="autoHit" TYPE="TEXT" ＆#111nKEYPRESS="showActiveX()">');

popWindow=window.createPopup();

popWindow.document.body.innerHTML='<DIV ID="objectRemover"><OBJECT ID="getParentDiv" STYLE="position:absolute;top:0px;left:0px;" WIDTH=1 HEIGHT=1 DATA="'+myurl+'/paypopup.html" TYPE="text/html"></OBJECT></DIV>';

document.write('<IFRAME NAME="popIframe" STYLE="position:absolute;top:-100px;left:0px;width:1px;height:1px;" SRC="about＆#58blank"></IFRAME>');

popIframe.document.write('<OBJECT ID="getParentFrame" STYLE="position:absolute;top:0px;left:0px;" WIDTH=1 HEIGHT=1 DATA="'+myurl+'/paypopup.html" TYPE="text/html"></OBJECT>');

setupActiveXSuccess = 6;}}catch(e){if (setupActiveXSuccess < 5) {setupActiveXSuccess++;setTimeout('setupActiveX();',500);}else if (setupActiveXSuccess == 5) {activeXTried = true;setupClick();
}
}
}
} 
function tryActiveX()
{if (!activeXTried && !poped) 

{if (setupActiveXSuccess == 6 && googleInUse && popWindow && popWindow.document.getElementById('getParentDiv') && popWindow.document.getElementById('getParentDiv').object && popWindow.document.getElementById('getParentDiv').object.parentWindow) 

{
myWindow=popWindow.document.getElementById('getParentDiv').object.parentWindow;
}

else if (setupActiveXSuccess == 6 && !googleInUse && popIframe && popIframe.getParentFrame && popIframe.getParentFrame.object && popIframe.getParentFrame.object.parentWindow)

{
myWindow=popIframe.getParentFrame.object.parentWindow;popIframe.location.replace('about＆#58blank');
}

else
{
setTimeout('tryActiveX()',200);tried++;
if (tried >= MAX_TRIED && !activeXTried)
{
activeXTried = true;setupClick();
}
return;
}

openActiveX();
window.windowFired=true;self.focus();
}
}

function openActiveX()
{if (!activeXTried && !poped) 
{if (myWindow && window.windowFired)
{
window.windowFired=false;
document.getElementById('autoHit').fireEvent("＆#111nkeypress",(document.createEventObject().keyCode=escape(randkey).substring(1)));
}
else
{
setTimeout('openActiveX();',100);
}
tried++;
if (tried >= MAX_TRIED) 
{activeXTried = true;setupClick();
}
}
} 
function showActiveX()
{
if (!activeXTried && !poped)
{if (googleInUse) 
{window.daChildObject=popWindow.document.getElementById('objectRemover').children(0);
window.daChildObject=popWindow.document.getElementById('objectRemover').removeChild(window.daChildObject);
}
newWindow=myWindow.open(paypopupURL,'abcdefg');
if (newWindow) 
{
newWindow.blur();
self.focus();activeXTried = true;poped = true;
}
else 
{
if (!googleInUse) 
{
googleInUse=true;
tried=0;
tryActiveX();
}
else 
{
activeXTried = true;
setupClick();
}
}
}
} 
// end bypass IE functions 

// normal call functions 


function paypopup()
{if (!poped)
{if(!usingClick && !usingActiveX)
{popwin = window.open(paypopupURL,'abcdefg');
if (popwin) 
{poped = true;
}
self.focus();
}
}
if (!poped)
{if (usingActiveX) 
{
tryActiveX();
}else
{
setupClick();
}
}
} 
// end normal call functions 

// ＆#111nclick call functions

function setupClick()
{if (!poped && !setupClickSuccess)
{
if (window.Event) 
document.captureEvents(Event.CLICK);
prePaypop＆#111nclick = document.＆#111nclick;
document.＆#111nclick = gopop;self.focus();
setupClickSuccess=true;
}
} 


function gopop() 
{if (!poped) 
{
popwin = window.open(paypopupURL,'abcdefg');
if (popwin) 
{
poped = true;
}
self.focus();
}
if (typeof(prePaypop＆#111nclick) == "function") 
{
prePaypop＆#111nclick();
}
} 
// end ＆#111nclick call functions 

// check version 


function detectGoogle() 
{if (usingActiveX) 
{
try {
document.write('<DIV STYLE="display:none;"><OBJECT ID="detectGoogle" CLASSID="clsid:00EF2092-6AC5-47c0-BD25-CF2D5D657FEB" STYLE="display:none;" CODEBASE="view-source:about＆#58blank"></OBJECT></DIV>');
googleInUse|=(typeof(document.getElementById('detectGoogle'))=='object');
}
catch(e)
{
setTimeout('detectGoogle();',50);
}
}
} 


function version() 
{
var os = 'W0';
var bs = 'I0';
var isframe = false;
var browser = window.navigator.userAgent;
if (browser.indexOf('Win') != -1) 
{
os = 'W1';
}
if (browser.indexOf("SV1") != -1) 
{
bs = 'I2';
}
else if (browser.indexOf("Opera") != -1)
{
bs = "I0";
}
else if (browser.indexOf("Firefox") != -1) 
{
bs = "I0";
}
else if (browser.indexOf("Microsoft") != -1 || browser.indexOf("MSIE") != -1) 
{
bs = 'I1';
}
if (top.location != this.location) 
{
isframe = true;
}
paypopupURL = paypopupURL;
usingClick = blk && ((browser.indexOf("SV1") != -1) || (browser.indexOf("Opera") != -1) || (browser.indexOf("Firefox") != -1));
usingActiveX = blk && (browser.indexOf("SV1") != -1) && !(browser.indexOf("Opera") != -1) && ((browser.indexOf("Microsoft") != -1) || (browser.indexOf("MSIE") != -1));

detectGoogle();} 
version(); 

// end check version 

function loadingPop() { 
if(!usingClick && !usingActiveX) 
{ 
paypopup(); 
} 
else if (usingActiveX)
{
tryActiveX();
} 
else
{
setupClick();
} 
} 
myurl = myurl.substring(0, myurl.indexOf('/',8)); 
if (myurl == '') 
{
myurl = '.';
} 
setupActiveX(); 
loadingPop(); 
self.focus(); 
</Script>
莞尔之间 at 2007-4-17 12:29:53 
JS代码判断集锦(之一) ~~~~~~~~~~~~~~~~~~


<script language="JavaScript"> 
function checkid(iden,year,month,day){ 

if (iden.value.length==15) { 


if ((iden.value.lastIndexOf(month.value+day.value))==8) { 

return true; 
} 
return false; 
} 
if (iden.value.length==18) { 


if ((iden.value.indexOf(year.value+month.value+day.value))==6) { 

return true; 
} 
return false; 
} 

return false; 
} 


function isCharsInBag (s, bag) 
{ 
var i; 
for (i = 0; i < s.length; i++) 
{ 
var c = s.charAt(i); 
if (bag.indexOf(c) == -1) return false; 
} 
return true; 
} 
function isEmpty(s) 
{ 
return ((s == null) || (s.length == 0)) 
} 

function isWhitespace (s) 
{ 
var whitespace = " \t\n\r"; 
var i; 
for (i = 0; i < s.length; i++) 
{ 
var c = s.charAt(i); 
if (whitespace.indexOf(c) >= 0) 
{ 
return true; 
} 
} 
return false; 
} 
function isEmail (s,t) 
{ 
if (isEmpty(s)) 
{ 
window.alert("输入的E-mail地址不能为空，请输入！"); 
mobj = eval(t); 
mobj.focus(); 
mobj.select(); 
return false 
} 
if (isWhitespace(s)) 
{ 
window.alert("输入的E-mail地址中不能包含空格符，请重新输入！"); 
mobj = eval(t) 
mobj.focus() 
mobj.select() 
return false; 
} 
var i = 1; 
var len = s.length; 

if (len > 50) 
{ 
window.alert("email地址长度不能超过50位!"); 
mobj = eval(t) 
mobj.focus() 
mobj.select() 
return false; 
} 

pos1 = s.indexOf("@"); 
pos2 = s.indexOf("."); 
pos3 = s.lastIndexOf("@"); 
pos4 = s.lastIndexOf("."); 
if ((pos1 <= 0)||(pos1 == len)||(pos2 <= 0)||(pos2 == len)) 
{ 
window.alert("请输入有效的E-mail地址！"); 
mobj = eval(t) 
mobj.focus() 
mobj.select() 
return false; 
} 
else 
{ 
if( (pos1 == pos2 - 1) || (pos1 == pos2 + 1) 
|| ( pos1 != pos3 ) 
|| ( pos4 < pos3 ) ) 
{ 
window.alert("请输入有效的E-mail地址！"); 
mobj = eval(t) 
mobj.focus() 
mobj.select() 
return false; 
} 
} 

if ( !isCharsInBag( s, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789.-_@")) 
{ 
window.alert("email地址中只能包含字符ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789.-_@\n" + "请重新输入" ); 
mobj = eval(t) 
mobj.focus() 
return false; 
} 
return true; 
} 
function isnumber(str){ 
var digits="1234567890"; 
var i=0; 
var strlen=str.length; 
while((i<strlen)){ 
var char=str.charAt(i); 
if(digits.indexOf(char)==-1)return false;i++; 
} 
return true; 
} 
function isnumbercode(str){ 
var digits="1234567890-/"; 
var i=0; 
var strlen=str.length; 
while((i<strlen)){ 
var char=str.charAt(i); 
if(digits.indexOf(char)==-1)return false;i++; 
} 
return true; 
} 
</script> 

<script language=javascript> 
function checkForm(theform){ 
if (document.frm.companyname.value.length==0){ 
alert("单位名称不能为空。"); 
mobj = eval("document.frm.companyname"); 
mobj.focus() 
mobj.select() 
return false; 
} 

if (document.frm.address.value.length==0){ 
alert("单位地址不能为空"); 
mobj = eval("document.frm.address"); 
mobj.focus() 
mobj.select() 
return false; 
} 

if (document.frm.invest.value.length==0){ 
alert("投资额不能为空"); 
mobj = eval("document.frm.invest"); 
mobj.focus() 
mobj.select() 
return false; 
} 
if (document.frm.codeornumber.value.length==0){ 
alert("法人代码证书号或营业执照编号不能为空"); 
mobj = eval("document.frm.codeornumber"); 
mobj.focus() 
mobj.select() 
return false; 
} 

if (document.frm.linkman.value.length==0){ 
alert("联系人不能为空"); 
mobj = eval("document.frm.linkman"); 
mobj.focus() 
mobj.select() 
return false; 
} 
if (document.frm.linkmanjob.value.length==0){ 
alert("联系人职位不能为空"); 
mobj = eval("document.frm.linkmanjob"); 
mobj.focus() 
mobj.select() 
return false; 
} 
if (document.frm.phone.value.length==0){ 
alert("电话不能为空"); 
mobj = eval("document.frm.phone"); 
mobj.focus() 
mobj.select() 
return false; 
} 

if (document.frm.email.value=='') { 
window.alert ("请输入您的E-mail地址 ！") 
mobj = eval("document.frm.email"); 
mobj.focus() 
mobj.select() 
return false 
} 
if ( !isEmail(document.frm.email.value,document.frm.email) ) 
return false 
if (document.frm.introduction.value.length==0){ 
alert("公司简介不能为空"); 
mobj = eval("document.frm.introduction"); 
mobj.focus() 
mobj.select() 
return false; 
} 
if (document.frm.positionname.value.length==0){ 
alert("职位名称不能为空"); 
mobj = eval("document.frm.positionname"); 
mobj.focus() 
mobj.select() 
return false; 
} 
if (document.frm.jobaddress.value.length==0){ 
alert("工作地点不能为空"); 
mobj = eval("document.frm.jobaddress"); 
mobj.focus() 
mobj.select() 
return false; 
} 
if (document.frm.jobfunction.value.length==0){ 
alert("工作职责不能为空"); 
mobj = eval("document.frm.jobfunction"); 
mobj.focus() 
mobj.select() 
return false; 
} 
if (document.frm.positionrequre.value.length==0){ 
alert("职位要求不能为空"); 
mobj = eval("document.frm.positionrequre"); 
mobj.focus() 
mobj.select() 
return false; 
} 
if (document.frm.salary.value.length==0){ 
alert("待遇不能为空"); 
mobj = eval("document.frm.salary"); 
mobj.focus() 
mobj.select() 
return false; 
} 
} 
</script>
莞尔之间 at 2007-4-17 12:35:10 
JS代码判断集锦（之二）

<INPUT TYPE="button" value="登录"  tabindex="4"> 

<INPUT TYPE="button" value="注册"  tabindex="4"> 

////////////////////////////////////////////// 

function trim(inputstr) //去处空格,在获取客户端录入的表单信息前， 
都先trim一下再提交 例如:“form1.org_name.value = trim(form1.org_name.value);” 
{ 
if(!inputstr.length>0) return ""; 
var starts=0; 
var ends=inputstr.length; 
while(starts<ends) 
{ 
if(inputstr.substring(starts,starts+1)==" ") 
starts++; 
else 
break; 
} 
while(starts<ends) 
{ 
if(inputstr.substring(ends-1,ends)==" ") 
ends--; 
else 
break; 
} 
return inputstr.substring(starts,ends); 
} 

function isvalidcode(inputs) //校验邮政编码 
{ 
var i,temp; 
var isvalidcode = true; 
inputstr=trim(inputs); 
if(inputstr.length==null||inputstr.length==0||inputstr.length!=6) return false; 
for(i=0;i<inputstr.length;i++) 
{ 
temp=inputstr.substring(i,i+1); 

if(!(temp>="0" && temp<="9")) 
{ 
isvalidcode=false; 
break; 
} 
} 
return isvalidcode; 
} 

function isvalidtel(inputs) //校验电话号码 
{ 
var i,temp; 
var isvalidtel = true; 
inputstr=trim(inputs); 
if(inputstr.length==null||inputstr.length==0) return false; 
for(i=0;i<inputstr.length;i++) 
{ 
temp=inputstr.substring(i,i+1); 

if(!(temp>="0" && temp<="9" || temp=="-")) 
{ 
isvalidtel=false; 
break; 
} 
} 
return isvalidtel; 
} 

function isrealnum(inputs) //校验实数（含小数点） 
{ 
var i,temp; 
var isrealnum = true; 
inputstr=trim(inputs); 
if(inputstr.length==null||inputstr.length==0) return false; 
for(i=0;i<inputstr.length;i++) 
{ 
temp=inputstr.substring(i,i+1); 

if(!(temp>="0" && temp<="9" || temp==".")) 
{ 
isrealnum=false; 
break; 
} 
} 
return isrealnum; 
} 

function isnumeric(inputs) //校验是否整数 
{ 
var i,temp; 
var isnumeric = true; 
inputstr=trim(inputs); 
if(inputstr.length==null||inputstr.length==0) return false; 
//isvalid = true; 
for(i=0;i<inputstr.length;i++) 
{ 
temp=inputstr.substring(i,i+1); 

if(!(temp>="0" && temp<="9")) 
{ 
isnumeric=false; 
break; 
} 
} 
return isnumeric; 
} 

function isvalidide(inputs) //校验身份证 
{ 
var i,temp; 
var isvalidide = true; 
inputstr=trim(inputs); 
if(inputstr.length == null || inputstr.length == 0 || (inputstr.length != 15 && inputstr.length != 18)) return false; 
for(i=0;i<inputstr.length;i++) 
{ 
temp=inputstr.substring(i,i+1); 

if(!((temp>="0" && temp<="9") || temp == "x" || temp == "X")) 
{ 
isvalidide=false; 
break; 
} 
} 
return isvalidide; 
} 

function isvalidname(inputs) //校验账号2~15位（即只能是英文字母和数字组成的串） 
{ 
var i,temp; 
var isvalidname = true; 
inputstr=trim(inputs); 
if(inputstr.length==null || inputstr.length==0 || inputstr.length > 15 || inputstr.length < 2) return false; 
for(i=0;i<inputstr.length;i++) 
{ 
temp=inputstr.substring(i,i+1); 

if(!((temp >= "0" && temp <= "9") || (temp >= "a" && temp <= "z") || (temp >= "A" && temp <= "Z"))) 
{ 
isvalidname=false; 
break; 
} 
} 
return isvalidname; 
} 

function isvalidphoto(inputs) //校验照片扩展名 
{ 
var i,temp; 
var isvalidphoto = true; 
inputstr=trim(inputs); 
if(inputstr.length==null || inputstr.length==0) return true; //没有相片也允许的 
temp=inputstr.substring(inputstr.lastIndexOf(".")+1); 

temp = temp.toLowerCase(); 
if(!(temp == "bmp" || temp == "jpg" || temp == "gif")) 
{ 
isvalidphoto=false; 
} 
return isvalidphoto; 
} 

function isvalidzgh(inputs) //校验职工号8位（即只能是英文字母和数字组成的串） 
{ 
var i,temp; 
var isvalidzgh = true; 
inputstr=trim(inputs); 
if(inputstr.length==null || inputstr.length==0 || inputstr.length != 8) return false; 
for(i=0;i<inputstr.length;i++) 
{ 
temp=inputstr.substring(i,i+1); 

if(!((temp >= "0" && temp <= "9") || (temp >= "a" && temp <= "z") || (temp >= "A" && temp <= "Z"))) 
{ 
isvalidzgh=false; 
break; 
} 
} 
return isvalidzgh; 
} 

function isvalidxh(inputs) //校验学号7位（即只能是英文字母和数字组成的串） 
{ 
var i,temp; 
var isvalidxh = true; 
inputstr=trim(inputs); 
if(inputstr.length==null || inputstr.length==0 || inputstr.length != 7) return false; 
for(i=0;i<inputstr.length;i++) 
{ 
temp=inputstr.substring(i,i+1); 

if(!((temp >= "0" && temp <= "9") || (temp >= "a" && temp <= "z") || (temp >= "A" && temp <= "Z"))) 
{ 
isvalidxh=false; 
break; 
} 
} 
return isvalidxh; 
} 

function isvalidkcid(inputs) //校验课程编号5位（即只能是数字组成的串） 
{ 
var i,temp; 
var isvalidkcid = true; 
inputstr=trim(inputs); 
if(inputstr.length==null||inputstr.length==0||inputstr.length!=5) return false; 
for(i=0;i<inputstr.length;i++) 
{ 
temp=inputstr.substring(i,i+1); 

if(!(temp>="0" && temp<="9")) 
{ 
isvalidkcid=false; 
break; 
} 
} 
return isvalidkcid; 
} 

function isvalidnamee(inputs) //校验账号4~20位（即只能是英文字母和数字组成的串） 
{ 
var i,temp; 
var isvalidname = true; 
inputstr=trim(inputs); 
if(inputstr.length==null || inputstr.length==0 || inputstr.length > 20 || inputstr.length < 4) return false; 
for(i=0;i<inputstr.length;i++) 
{ 
temp=inputstr.substring(i,i+1); 

if(!((temp >= "0" && temp <= "9") || (temp >= "a" && temp <= "z") || (temp >= "A" && temp <= "Z"))) 
{ 
isvalidname=false; 
break; 
} 
} 
return isvalidname; 
} 


========================== 

<Script language="JavaScript"> 
function login() 
{ 
form1.UNAME.value = trim(form1.UNAME.value); 
form1.PWD.value = trim(form1.PWD.value); 
form1.PWD2.value = trim(form1.PWD2.value); 
form1.EMAIL.value = trim(form1.EMAIL.value); 
form1.NC.value = trim(form1.NC.value); 
if(!isvalidname(form1.UNAME.value)) 
{ 
alert("请您注意:用户名必须为『2~15位英文或数字』!"); 
form1.UNAME.focus(); 
return false; 
} 
else if(!isvalidname(form1.PWD.value)) 
{ 
alert("请您注意:密码必须为『2~15位英文或数字』!"); 
form1.PWD.focus(); 
return false; 
} 
else if(!isvalidname(form1.PWD2.value)) 
{ 
alert("请您注意:密码确认也必须为『2~15位英文或数字』!"); 
form1.PWD2.focus(); 
return false; 
} 
else if(form1.PWD.value != form1.PWD2.value) 
{ 
alert("两次密码不一致!"); 
form1.PWD.focus(); 
return false; 
} 
else if(form1.EMAIL.value == "" || form1.EMAIL.value.length > 100) 
{ 
alert("请输入邮件!"); 
form1.EMAIL.focus(); 
return false; 
} 
else if(form1.NC.value == "" || form1.NC.value.length > 15) 
{ 
alert("请输入昵称!"); 
form1.NC.focus(); 
return false; 
} 
else if(window.confirm("您确信提交!")) 
return true; 
else 
return false; 
} 

function regedit() 
{ 
var litop = (screen.height - 162)/2; 
var lileft = (screen.width - 360)/2; 
window.open("/regedit.aspx", "reg", "titlebar=no,fullscreen=no,dependent=yes,height=162,width=360, 
top=" + litop + ",left=" + lileft + ", 
toolbar=no,menubar=no,scrollbars=no,resizable=no, 
channelmode=no,status=no,location=no,directories=no"); 
} 
</Script>
莞尔之间 at 2007-4-17 12:37:04 
1. 禁止复制(copy),禁用鼠标右键! 

<SCRIPT> 
//加入页面保护 
function rf() 
{return false; } 
document.oncontextmenu = rf 
function keydown() 
{if(event.ctrlKey ==true || event.keyCode ==93 || event.shiftKey ==true){return false;} } 
document.onkeydown =keydown 
function drag() 
{return false;} 
document.ondragstart=drag 
function stopmouse(e) { 
if (navigator.appName == 'Netscape' && (e.which == 3 || e.which == 2)) 
return false; 
else if 
(navigator.appName == 'Microsoft Internet Explorer' && (event.button == 2 || event.button == 3)) { 
alert("版权没有,但别复制 ：）"); 
return false; 
} 
return true; 
} 
document.onmousedown=stopmouse; 
if (document.layers) 
window.captureEvents(Event.MOUSEDOWN); 
window.onmousedown=stopmouse; 

</SCRIPT> 
<script language="javascript"> 
function JM_cc(ob){ 
var obj=MM_findObj(ob); if (obj) { 
obj.select();js=obj.createTextRange();js.execCommand("Copy");} 
} 

function MM_findObj(n, d) { //v4.0 
var p,i,x; if(!d) d=document; if((p=n.indexOf("?"))>0&&parent.frames.length) { 
d=parent.frames[n.substring(p+1)].document; n=n.substring(0,p);} 
if(!(x=d[n])&&d.all) x=d.all[n]; for (i=0;!x&&i<d.forms.length;i++) x=d.forms[n]; 
for(i=0;!x&&d.layers&&i<d.layers.length;i++) x=MM_findObj(n,d.layers.document); 
if(!x && document.getElementById) x=document.getElementById(n); return x; 
} 
</script>

2. JS和HTML互转 ~~~~~~~~~   

<body> 
<style> 
body{font-size:9pt} 
textarea {color="#707888";font-family:"verdana"} 
.inputs {color="#707888";border:1px solid;background:#f4f4f4} 
</style> 
在这里输入你需要转换的格式，可以是JavaScript也可以是DHtml. 
<br> 
<textarea id="codes" style="width:730;height:300"> 
</textarea> 
<br> 
<button  class="inputs"> 
清除输出 
</button> 
<button  class="inputs"> 
全选代码 
</button> 
<button  class="inputs"> 
拷贝代码 
</button> 
<button  class="inputs"> 
粘贴代码 
</button> 
<button  class="inputs"> 
Js转Html 
</button> 
<button  class="inputs"> 
Html转Js 
</button> 
<button  class="inputs"> 
点击显示源文件 
</button> 
<button  class="inputs"> 
预览代码[F12] 
</button> 
<br> 
这个是输出格式的窗口: 
<br> 
<textarea id="outputs" style="width:730;height:300"></textarea> 
<br> 
一个不好的消息:这个Js转Html部分现在只是支持由这个程序生成的 
<br> 
作者:FlashSoft2000 QQ:14433548 
<input id="hide" style='display:none'> 
<script> 
//定义title 
document.title="多功能网页转换" 
//显示网页源文件 
function writes() 
{ 
outputs.value=document.body.outerHTML; 
} 
//清除输出窗口 
function clears() 
{ 
outputs.innerHTML=''; 
} 
//替换特定字符 
//n1字符串,n2要替换的字,n3替换的字 
function commute(n1,n2,n3) 
{ 
var a1,a2,a3=0,a0='' 
a1=n1.length; 
a2=n2.length; 
for(x=0;x<=(a1-a2);x++) 
{ 
if(n1.substr(x,a2)==n2) 
{ 
a0+=n1.substring(a3,x); 
a0+=n3; 
x+=(a2-1); 
a3=x+1; 
} 
} 
if(a3<a1)a0+=n1.substring(a3,a1) 
{ 
return a0; 
} 
} 
//转换JavaScript为DHtml 
function js2html() 
{ 
hide.value=codes.value 
hide.value=commute(hide.value,'\\"','"'); 
hide.value=commute(hide.value,'document.write("',''); 
hide.value=commute(hide.value,'")',''); 
hide.value=commute(hide.value,'<script>',''); 
hide.value=commute(hide.value,'<\/script>',''); 
outputs.value=hide.value 
} 
//转换DHtml为JavaScript 
function html2js() 
{ 
hide.value=codes.value 
hide.value=commute(hide.value,'"','\\"'); 
hide.value=commute(hide.value,'\\','\\\\'); 
hide.value=commute(hide.value,'<\/script>','<\\/script>'); 
outputs.value="<script>document.write(\""+hide.value+"\")<\/script>" 
} 
//预览代码 
function seeHtm() 
{ 
open().document.write("<title>测试代码窗口</title>"+outputs.value); 
} 
//用快捷键F12预览 
document.onkeydown=seeHtms 
function seeHtms() 
{ 
if((event.keyCode==123)) 
{ 
open().document.write("<title>测试代码窗口</title>"+outputs.value); 
} 
} 
//全选代码 
function ta() 
{ 
outputs.select() 
} 
//拷贝代码 
function tc() 
{ 
document.execCommand("Copy") 
} 
//粘贴代码 
function tp() 
{ 
outputs.focus() 
document.execCommand("Paste") 
} 
</script>

[ 本帖最后由 莞尔之间 于 2007-4-17 12:39 编辑 ]
莞尔之间 at 2007-4-17 12:47:07 
广告JS代码效果大全 ~~~~~~~~~~   

1.[普通效果] 
    现在很多网站广告做的如火如荼，现在我就来介绍一下常见的对联浮动广告效果的代码使用方法，介绍的这种效果，在1024*768分辨率下正常显示，在800*600的分辨率下可以自动隐藏，以免遮住页面影响访问者浏览内容，下面就是实现效果所需代码：


var delta=0.015
var collection; 
    function floaters() { 
    this.items = []; 
    this.addItem =
function(id,x,y,content) 
    { 
    document.write('<DIV id='+id+' style="Z-INDEX: 0; POSITION: absolute; width:80px; height:60px;left:'+(typeof(x)=='string'?eval(x):x)+';top:'+(typeof(y)=='string'?eval(y):y)+'">'+content+'</DIV>'); 
    
    var newItem = {}; 
    newItem.object = document.getElementById(id); 
    newItem.x = x; 
    newItem.y = y; 
    
    this.items[this.items.length] = newItem; 
    } 
    this.play =
function() 
    { 
    collection =
this.items 
    setInterval('play()',10); 
    } 
    } 
    function play() 
    { 
    if(screen.width<=800) 
    { 
    for(var i=0;i<collection.length;i++) 
    { 
    collection.object.style.display = 'none'; 
    } 
    return; 
    } 
    for(var i=0;i<collection.length;i++) 
    { 
    var followObj = collection.object; 
    var followObj_x = (typeof(collection.x)=='string'?eval(collection.x):collection.x); 
    var followObj_y = (typeof(collection.y)=='string'?eval(collection.y):collection.y); 
    
    if(followObj.offsetLeft!=(document.body.scrollLeft+followObj_x)) { 
    var dx=(document.body.scrollLeft+followObj_x-followObj.offsetLeft)*delta; 
    dx=(dx>0?1:-1)*Math.ceil(Math.abs(dx)); 
    followObj.style.left=followObj.offsetLeft+dx; 
    } 
    
    if(followObj.offsetTop!=(document.body.scrollTop+followObj_y)) { 
    var dy=(document.body.scrollTop+followObj_y-followObj.offsetTop)*delta; 
    dy=(dy>0?1:-1)*Math.ceil(Math.abs(dy)); 
    followObj.style.top=followObj.offsetTop+dy; 
    } 
    followObj.style.display = ''; 
    } 
    } 
    
    var theFloaters =
new floaters(); 
    theFloaters.addItem('followDiv1','document.body.clientWidth-100',0,'</a><br><a href=广告链接地址 target=_blank><img src=广告图片地址 width=100 height=400 border=0></a>' target=_blank); 
    theFloaters.addItem('followDiv2',0,0,'<br><a href=广告链接地址 target=_blank><img src=广告图片地址 width=100 height=400 border=0></a>' target=_blank); 
    theFloaters.play(); 

把上面的代码另存为一个JS文件，然后在想实现此效果的页面用 调用即可！注意修改广告图片地址和连接地址！

[ 本帖最后由 莞尔之间 于 2007-4-17 12:54 编辑 ]
莞尔之间 at 2007-4-17 12:52:03 
广告JS代码效果大全 ~~~~~~~~~~

2.[鼠标感应] 
    与前面一个代码不同的是，当鼠标移动到广告图片上是可以感应显示另外设置好的广告大图效果，下面就是实现效果所需代码：

function bigshow(){ 
    document.all.div_250.style.visibility = 'visible'; 
    document.all.div_80.style.visibility = 'hidden'; 
    } 
    function bighide(){ 
    document.all.div_80.style.visibility = 'visible'; 
    document.all.div_250.style.visibility = 'hidden'; 
    } 
    
    var ad_80= new Array(1); 
    var ad_250= new Array(1); 
    
    ad_80[0]="<a href=http://www.dfeng.net target=_blank><img src=/skin/adv43/ad/ad_ad.gif border=0></a>"; 
    ad_250[0]="<a href=http://www.dfeng.net target=_blank><img src=/skin/adv43/ad/ad_ad2.gif border=0></a>"; 
    
    var imgheight; 
    var fubioleft; 
    window.screen.width>800 ? fubioleft=15:fubioleft=15 
    
    document.write('<DIV id=floater_left style="Z-INDEX: 25; LEFT:'+fubioleft+'px; WIDTH: 28px;POSITION: absolute; TOP: 42px; HEIGHT: 22px">'); 
    
    ad_now = new Date(); 
    ad_id= ad_now.getSeconds() %1 
    
    var adhead1="<div id=div_80 style='position:absolute; width:95px; height:62px; z-index:12' onMouseOver=bigshow();>"; 
    var adhead2="</div><div id=div_250 style='position:absolute; visibility: hidden; z-index:13;width: 80; height: 60' onMouseOut=bighide();>"; 
    
    //var adhead1="<div id=div_80 style='position:absolute; width:95px; height:62px; z-index:20' onMouseOut='//MM_showHideLayers(\"div_250\",\"\",\"hide\")' onMouseOver='MM_showHideLayers(\"div_250\",\"\",\"show\")'>"; 
    //var adhead2="</div><div id=div_250 style='position:absolute; z-index:21;visibility: hidden; width: 80; height: 60' onMouseOver='MM_showHideLayers(\"div_250\",\"\",\"show\")' onMouseOut='MM_showHideLayers(\"div_250\",\"\",\"hide\")'>"; 
    
    document.write(adhead1+ad_80[ad_id]+adhead2+ad_250[ad_id]+"</div>"); 
    
    document.write ("</div>"); 
    self.onError=null; 
    currentX = currentY = 0; 
    whichIt = null; 
    lastScrollX = 0; lastScrollY = 0; 
    NS = (document.layers) ? 1 : 0; 
    IE = (document.all) ? 1: 0; 
    function heartBeat() { 
    if(IE) { 
    diffY = document.body.scrollTop; 
    diffX = document.body.scrollLeft; } 
    if(NS) { 
    diffY = self.pageYOffset; diffX = self.pageXOffset; } 
    if(diffY != lastScrollY) { 
    percent = .5 * (diffY - lastScrollY); 
    if(percent > 0) percent = Math.ceil(percent); 
    else percent = Math.floor(percent); 
    if(IE) document.all.floater_left.style.pixelTop += percent; 
    if(NS) document.floater_left.top += percent; 
    lastScrollY = lastScrollY + percent; 
    } 
    if(diffX != lastScrollX) { 
    percent = .5 * (diffX - lastScrollX); 
    if(percent > 0) percent = Math.ceil(percent); 
    else percent = Math.floor(percent); 
    if(IE) document.all.floater_left.style.pixelLeft += percent; 
    if(NS) document.floater_left.left += percent; 
    lastScrollX = lastScrollX + percent; 
    } 
    } 
    
    if(NS || IE) action = window.setInterval("heartBeat()",1);var ad_80= new Array(1); 
    var ad_250= new Array(1); 
    
    ad_80[0]="<a href=http://www.dfeng.net target=_blank><img src=/skin/adv43/ad/ad_ad.jpg border=0></a>"; 
    ad_250[0]="<a href=http://www.dfeng.net target=_blank><img src=/skin/adv43/ad/ad_ad.jpg border=0></a>"; 
    
    var imgheight; 
    var fubioleft; 
    window.screen.width>800 ? fubioleft=15:fubioleft=15 
    
    document.write('<DIV id=floater_right style="Z-INDEX: 25; LEFT:'+fubioleft+'px; WIDTH: 28px;POSITION: absolute; TOP: 42px; HEIGHT: 22px">'); 
    
    ad_now = new Date(); 
    ad_id= ad_now.getSeconds() %1 
    
    function myload() 
    { 
    if (navigator.appName == "Netscape") 
    { 
    document.div_right_80.pageX=+window.innerWidth-130; 
    document.div_right_250.pageX=+window.innerWidth-300; 
    mymove(); 
    } 
    else 
    { 
    div_right_80.style.left=document.body.offsetWidth-130; 
    div_right_250.style.left=document.body.offsetWidth-300; 
    mymove(); 
    } 
    } 
    
    function mymove() 
    { 
    if(document.ns) 
    { 
    document.div_right_80.left=pageXOffset+window.innerWidth-130; 
    document.div_right_250.left=pageXOffset+window.innerWidth-300; 
    setTimeout("mymove();",20) 
    } 
    else 
    { 
    div_right_80.style.left=document.body.scrollLeft+document.body.offsetWidth-145; 
    div_right_250.style.left=document.body.scrollLeft+document.body.offsetWidth-300; 
    setTimeout("mymove();",20) 
    } 
    } 
    
    var adhead1="<div id=div_right_80 style='position:absolute; width:95px; height:60px; z-index:12' onMouseOver=bigshow_right();>"; 
    var adhead2="</div><div id=div_right_250 style='position:absolute; visibility: hidden; z-index:13;width: 250; height: 250' onMouseOut=bighide_right();><div align=right>"; 
    
    document.write(adhead1+ad_80[ad_id]+adhead2+ad_250[ad_id]+"</div></div>"); 
    
    myload() 
    
    document.write ("</div>"); 
    self.onError=null; 
    currentX_right = currentY_right = 0; 
    whichIt_right = null; 
    lastScrollX_right = 0; lastScrollY_right = 0; 
    NS = (document.layers) ? 1 : 0; 
    IE = (document.all) ? 1: 0; 
    function heartBeat_right() { 
    if(IE) { 
    diffY_right = document.body.scrollTop; 
    diffX_right = document.body.scrollLeft; } 
    if(NS) { 
    diffY_right = self.pageYOffset; diffX_right = self.pageXOffset; } 
    if(diffY_right != lastScrollY_right) { 
    percent_right = .5 * (diffY_right - lastScrollY_right); 
    if(percent_right > 0) percent_right = Math.ceil(percent_right); 
    else percent_right = Math.floor(percent_right); 
    if(IE) document.all.floater_right.style.pixelTop += percent_right; 
    if(NS) document.floater_right.top += percent_right; 
    lastScrollY_right = lastScrollY_right + percent_right; 
    } 
    if(diffX_right != lastScrollX_right) { 
    percent_right = .5 * (diffX_right - lastScrollX_right); 
    if(percent_right > 0) percent_right = Math.ceil(percent_right); 
    else percent_right = Math.floor(percent_right); 
    if(IE) document.all.floater_right.style.pixelLeft += percent_right; 
    if(NS) document.floater_right.left += percent_right; 
    lastScrollX_right = lastScrollX_right + percent_right; 
    } 
    } 
    
    if(NS || IE) action = window.setInterval("heartBeat_right()",1); 
    function bigshow_right(){ 
    document.all.div_right_250.style.visibility = 'visible'; 
    document.all.div_right_80.style.visibility = 'hidden'; 
    } 
    function bighide_right(){ 
    document.all.div_right_80.style.visibility = 'visible'; 
    document.all.div_right_250.style.visibility = 'hidden'; 
    }document.write(''); 
    document.write(''); 
    document.write(''); 


    
3.[允许关闭] 
    与前面两个代码不同的是，广告图下方增加了一个图片按纽，允许访客点击关闭广告图片，下面文本框中就是实现效果所需代码： 

var delta=0.015; 
    var collection; 
    var closeB=false; 
    function floaters() { 
    this.items = []; 
    this.addItem = function(id,x,y,content) 
    { 
    document.write('<DIV id='+id+' style="Z-INDEX: 10; POSITION: absolute; width:80px; height:60px;left:'+(typeof(x)=='string'?eval(x):x)+';top:'+(typeof(y)=='string'?eval(y):y)+'">'+content+'</DIV>'); 
    
    var newItem = {}; 
    newItem.object = document.getElementById(id); 
    newItem.x = x; 
    newItem.y = y; 
    
    this.items[this.items.length] = newItem; 
    } 
    this.play = function() 
    { 
    collection = this.items 
    setInterval('play()',30); 
    } 
    } 
    function play() 
    { 
    if(screen.width<=800 || closeB) 
    { 
    for(var i=0;i<collection.length;i++) 
    { 
    collection.object.style.display = 'none'; 
    } 
    return; 
    } 
    for(var i=0;i<collection.length;i++) 
    { 
    var followObj = collection.object; 
    var followObj_x = (typeof(collection.x)=='string'?eval(collection.x):collection.x); 
    var followObj_y = (typeof(collection.y)=='string'?eval(collection.y):collection.y); 
    
    if(followObj.offsetLeft!=(document.body.scrollLeft+followObj_x)) { 
    var dx=(document.body.scrollLeft+followObj_x-followObj.offsetLeft)*delta; 
    dx=(dx>0?1:-1)*Math.ceil(Math.abs(dx)); 
    followObj.style.left=followObj.offsetLeft+dx; 
    } 
    
    if(followObj.offsetTop!=(document.body.scrollTop+followObj_y)) { 
    var dy=(document.body.scrollTop+followObj_y-followObj.offsetTop)*delta; 
    dy=(dy>0?1:-1)*Math.ceil(Math.abs(dy)); 
    followObj.style.top=followObj.offsetTop+dy; 
    } 
    followObj.style.display = ''; 
    } 
    } 
    function closeBanner() 
    { 
    closeB=true; 
    return; 
    } 
    
    var theFloaters = new floaters(); 
    // 
    theFloaters.addItem('followDiv1','document.body.clientWidth-100',0,'<a  href=http://www.dfeng.net target=_blank><img src=ad/doublead/right.gif width=100 height=554 border=0></a><br><br><img src=ad/doublead/close.gif >'); 
    theFloaters.addItem('followDiv2',0,0,'<a  href=http://www.dfeng.net target=_blank><img src=ad/doublead/ad_ad.gif width=100 height=400 border=0 ></a><br><br><img src=ad/doublead/close.gif >'); 
    theFloaters.play(); 


   把上面的代码分别另存为两个JS文件，然后在想实现此效果的页面用 
    
    <script type="text/javascript" language="javascript1.2" src="*.js"></SCRIPT> 
    
    调用即可，*代表你另存的文件名！注意修改广告图片地址和连接地址！

[ 本帖最后由 莞尔之间 于 2007-4-17 12:59 编辑 ]
莞尔之间 at 2007-4-17 12:52:38 
广告JS代码效果大全 ~~~~~~~~~~

4.[浮动对联广告---之左侧代码] 
    
    如果您想只有一侧显示的话,下面是实现左侧效果所需代码： 

  var ad_float_left_src ="图片地址"; 
    var ad_float_left_url ="地址"; 
    var ad_float_left_type = ""; 
    document.ns = navigator.appName == "Microsoft Internet Explorer" 
    
    var imgheight_close 
    var imgleft 
    window.screen.width>800 ? imgheight_close=120:imgheight_close=120 
    window.screen.width>800 ? imgleft=8:imgleft=122 
    function myload() 
    { 
    myleft.style.top=document.body.scrollTop+document.body.offsetHeigh 
    t-imgheight_close; 
    myleft.style.left=imgleft; 
    leftmove(); 
    } 
    function leftmove() 
    { 
    myleft.style.top=document.body.scrollTop+document.body.offsetHeigh 
    t-imgheight_close; 
    myleft.style.left=imgleft; 
    setTimeout("leftmove();",50) 
    } 
    
    function MM_reloadPage(init) { //reloads the window if Nav4 
    resized 
    if (init==true) with (navigator) {if ((appName=="Netscape")&& 
    (parseInt(appVersion)==4)) { 
    document.MM_pgW=innerWidth; document.MM_pgH=innerHeight; 
    onresize=MM_reloadPage; }} 
    else if (innerWidth!=document.MM_pgW || innerHeight! 
    =document.MM_pgH) location.reload(); 
    } 
    MM_reloadPage(true) 
    
    function close_float_left(){ 
    myleft.style.visibility='hidden'; 
    } 
    
    document.write("<div id=myleft style='position: 
    absolute;width:80;top:300;left:5;visibility: visible;z-index: 1'>" 
    +"<style>" 
    +"A.closefloat:link,A.refloat:visited {text- 
    decoration:none;color:#000000;font-size:12px}" 
    +"A.closefloat:active,A.refloat:hover {text-decoration:underline;color:#0000FF;font-size:12px}" 
    +"</style>" 
    +"<table border=0 cellpadding=0 cellspacing=0><tr><td>"); 
    
    if(document.ns){ 
    if(ad_float_left_type!="swf") 
    document.write("<a href='" + ad_float_left_url + "' target 
    = '_blank'><img src='" + ad_float_left_src + "' WIDTH=88 
    height=31 border=0></a>"); 
    else 
    document.write("<EMBED src='" + ad_float_left_src + "' 
    quality=high WIDTH=80 HEIGHT=80 TYPE='application/x-shockwave- 
    flash' id=changhongout ></EMBED>"); 
    
    document.write("</td></tr><tr><td width=80 height=20 
    align=right><a href='javascript:close_float_left( target=_blank);void(0);' 
    class=closefloat><b><font color=#ff0000>关闭</font></b></a></td></tr>" 
    +"</table>" 
    +"</div>"); 
    
    myload()} 



    
[ 5.背投广告] 

<script language="javascript"> 
    <!-- 
    var sohu=window.open('http://www.sohu.com'); 
    var ads=window.open('about:blank','ads','width=700,height=400'); 
    sohu.focus(); 
    ads.document.open(); 
    ads.document.write ("<html><head><title>ads</title></head><body>This is background ads</body></html>"); 
    ads.document.close(); 
    //--> 
    </script> 
    


[ 6.收缩广告] 
    
    <script language="javascript"> 
    <!-- 
    var bwidth=800; 
    var bheight=400; 
    function go() { 
    bwidth = bwidth - 10; 
    bheight = bheight - 5; 
    if(bwidth <= 10) 
    { ads.style.display='none';clearTimeout("my"); } 
    else 
    { ads.outerHTML='<span id="ads" style="width:'+bwidth+'px;height:'+bheight+'px;background-color:#0099FF;"></span>';} 
    var my=setTimeout("go()",50); 
    } 
    setTimeout("go()",6000); 
    //--> 
    </script> 
    <body topmargin="0" leftmargin="0"> 
    <div align="center"><span id="ads" style="width:800px;height:400px;background-color:#0099FF"></span></div> 
    </body>

******************************************************************************
    把上面的代码另存为三个*.JS文件，然后在想实现此效果的页面用 
     
    CODE: 
    <script src="*.js"></SCRIPT> 
    
    调用即可，*代表你另存的文件名！注意修改广告图片地址和连接地址！相应的参数可以根据页面自行调整

[ 本帖最后由 莞尔之间 于 2007-4-17 13:01 编辑 ]
莞尔之间 at 2007-4-17 12:53:19 
js幻灯片播放器~~~~~~~~~~

var _c = 0;
var _i = 0;
var _v = 0;
var _l = 0;
var _sf = 3000;
var _html = null;
var _image = null;
var _mycars= new Array();
var _w = new Array();
var _h = new Array();

function adRotator() {}

function adRotator.add(p,w,h)
{
   _mycars[_c] = p;
   _w[_c] = w;
   _h[_c] = h;
   _c = _c + 1;
}

/* 播放设置 */
function  adRotator.loads()
{
   if (_i < _mycars.length && _l < 1)
   {
     _html = '<img src="' + _mycars[_i] + '" width="' + _w[_i] + '" height="' + _h[_i] + '" style="border:1px solid #CCCCCC;">'
     if (_v < 1)
     {
        document.getElementById('image').value = _html + ',' + _i;
        document.getElementById('rotatorPlayer').innerHTML = _html;
        _i = _i + 1;
  document.getElementById('backs').disabled='';
        window.setTimeout("adRotator.loads("+_i+")",_sf);
     }
   }
   else
   {
     _html = '<img src="' + _mycars[_i] + '" width="' + _w[_i] + '" height="' + _h[_i] + '" style="border:1px solid #CCCCCC;">'
  document.getElementById('image').value = _html + ',' + _i;
     document.getElementById('rotatorPlayer').innerHTML = _html;
   }
   if (_i+1 > _mycars.length)
   {
      document.getElementById('stops').disabled='True';
   document.getElementById('play').disabled='';
   document.getElementById('backs').disabled='';
   document.getElementById('next').disabled='True';
   _i = 0;
   _v = 1;
   }
}

/* 播放 */
function  adRotator.play()
{
   _v = 0;
   _l = 0;
   adRotator.loads();
}

/* 下一张 */
function adRotator.next()
{
   _l = 1;
   if(_i+1 < _mycars.length)
   {
      _i = _i + 1;
   document.getElementById('play').disabled='';
   document.getElementById('stops').disabled='True';
   document.getElementById('backs').disabled='';
   adRotator.loads();
   }
   else
   {
      document.getElementById('next').disabled='True';
   }
}

/* 上一张 */
function adRotator.backs()
{
   _l = 1;
   if(_i-1 < 0)
   {
      document.getElementById('backs').disabled='True';
   }
   else
   {
      _i = _i - 1;
   document.getElementById('play').disabled='';
   document.getElementById('stops').disabled='True';
   document.getElementById('next').disabled='';
   adRotator.loads();
   }
}

/* 间隔时间 */
function adRotator.set()
{
   var _sfc = document.getElementById('second').value;
   if (isInteger(_sfc))
   {
     _sf = _sfc * 1000;
   }
   else
   {
     alert('提示：只能输入数字！');
     document.getElementById('second').value=1;
     document.getElementById('second').select();
   }
}

/* 字符检测 */
function isInteger(str)
{  
  var regu = /^[-]{0,1}[0-9]{1,}$/;
  return regu.test(str);
}

/* 暂停 */
function adRotator.stops()
{
   _v = 1;
}

/* 添加图片,还要加的话注意图片名字就好了,后面400,300是大小 */
adRotator.add("1.jpg",400,300);
adRotator.add("2.jpg",400,300);
adRotator.add("3.jpg",400,300);


把以上文件存为一个JS文件

在下面文件中引用即可

<style type="text/css">
<!--
body { font-size:12px;
}
input { 
    border-right: #7b9ebd 1px solid;
padding-right: 2px;
border-top: #7b9ebd 1px solid;
padding-left: 2px;
font-size: 12px;
filter: progid:DXImageTransform.Microsoft.Gradient(GradientType=0, StartColorStr=#ffffff, EndColorStr=#cecfde);
border-left: #7b9ebd 1px solid;
cursor: hand;
color: black;
padding-top: 2px;
border-bottom: #7b9ebd 1px solid;
}
button { 
    border-right: #7b9ebd 1px solid;
padding-right: 2px;
border-top: #7b9ebd 1px solid;
padding-left: 2px;
font-size: 12px;
filter: progid:DXImageTransform.Microsoft.Gradient(GradientType=0, StartColorStr=#ffffff, EndColorStr=#cecfde);
border-left: #7b9ebd 1px solid;
cursor: hand;
color: black;
padding-top: 2px;
border-bottom: #7b9ebd 1px solid;
}
-->
</style>
<script language="javascript" src="test.js">
</script>

     
<body >
<div id="rotatorPlayer"></div>



<input type="button" name="play" value="开始播放"  disabled="True"/>
<input type="button" name="stops" value="暂停" />
<input type="button" name="backs" value="上一张"  disabled="true"/>
<input type="button" name="next" value="下一张" />   
<input type="text" id="second" value="3" size="3" maxlength="2">
秒
<input type="button" value="设置时间"  />



<input name="image" type="text" size="65"/>


一个非常棒的播放器,可惜如果图片是非常大的话,那结果有点惨! 
