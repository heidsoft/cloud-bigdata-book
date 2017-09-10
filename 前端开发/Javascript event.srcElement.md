Javascript event.srcElement srcElement 是Dom事件中的事件最初指派到的元素。 比如有一个div，里面有一个按钮。你响应div的onclick事件，但实际上，你单击的只是它内部的按钮，那么，srcElement指向的，就是那个按钮。srcElement只在IE中有效。在Opera系列浏览器中对应的属性是target给你一个IE下使用的例子。<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd"><html xmlns="http://www.w3.org/1999/xhtml" ><head>    <title>无标题页</title>    <script type="text/javascript">    function onLoad()    {        document.getElementById( "myDiv" ).attachEvent( "onclick", on_myDivClick );    }    function onUnLoad()    {        document.getElementById( "myDiv" ).detachEvent( "onclick", on_myDivClick );    }    function on_myDivClick( e )    {        if( !e ) e = window.event;        alert( "event: " + e.type + ", fromElement = " + e.srcElement.id );    }    </script></head><body onload="onLoad()" onunload="onUnLoad()"><div id="myDiv">    <input type="button" id="btn1" value="button1" />    <input type="button" id="btn2" value="button2" /></div></body></html>
Javascript event.srcElement

感觉这个挺好，可以捕获当前事件作用的对象，如event.srcElement.tagName可以捕获活动标记名称。
注意获取的标记都以大写表示，如"TD","TR","A"等。所以把看过的一些抄下来，不记得的时候再来看看。

<script type="text/javascript">
function tdclick(){
if(event.srcElement.tagName.toLowerCase()=='td')
alert("行："+(event.srcElement.parentNode.rowIndex+1)+"列："+(event.srcElement.cellIndex+1));
}
</script>

event.srcElement从字面上可以看出来有以下关键字：事件,源     他的意思就是：当前事件的源，

我们可以调用他的各种属性 就像:document.getElementById("")这样的功能，

经常有人问 firefox 下的 event.srcElement 怎么用，在此详细说明：

IE下,event对象有srcElement属性,但是没有target属性;Firefox下,event对象有target属性,但是没有srcElement属性.但他们的作用是相当的，即：

firefox 下的 event.target = IE 下的 event.srcElement

解决方法:使用obj(obj = event.srcElement ? event.srcElement : event.target;)来代替IE下的event.srcElement或者Firefox下的event.target.

以下是几种常见到的情况(IE下测试通过，如要测试firefox下的event.target可以修改并保存代码后用firefox打开测试)：


--------------------------------------------------------------------------------

 

event.srcElement.X显示一个控件的各种属性

<div id="div_001">

    <form id="form_001">

        <input type="button" id="button_001_id" name="button_001_Name" value=" 单击查看" class="button_001_Class" onclick="Get_srcElement(this)">

    </form>

</div>

<script>...

function Get_srcElement()

...{

var srcElement=""

srcElement = srcElement + " " +  "event.srcElement.id : " + event.srcElement.id

srcElement = srcElement + " " +  "event.srcElement.tagName : " + event.srcElement.tagName

srcElement = srcElement + " " +  "event.srcElement.type : " + event.srcElement.type

srcElement = srcElement + " " +  "event.srcElement.value : " + event.srcElement.value

srcElement = srcElement + " " +  "event.srcElement.name : " + event.srcElement.name

srcElement = srcElement + " " +  "event.srcElement.className : " + event.srcElement.className

srcElement = srcElement + " " +  "event.srcElement.parentElement.id : " + event.srcElement.parentElement.id

srcElement = srcElement + " " +  "event.srcElement.getattribute : " + event.srcElement.getAttribute

alert(srcElement)

}

</script>


--------------------------------------------------------------------------------

 

event.srcElement.selectedIndex一般使用在select对像上：

<select name="selectname" onchange="alert(event.srcElement.options[event.srcElement.selectedIndex].value)" >

  <option value="001">1</option>

  <option value="002">2</option>

  <option value="003">3</option>

  <option value="004">4</option>

  <option value="005">5</option>

</select>


--------------------------------------------------------------------------------

 

第一个子标签为

event.srcElement.firstChild

最后个一个是

event.srcElement.lastChild

当然也可以用

event.srcElement.children[i]

event.srcElement.ChildNode[i]

至于event.srcElement.parentElement是指在鼠标所在对象的上一个对象．

<div onclick="Get_srcElement()">

<div id="001" style="border:1px solid red;width:100%;height:500px"> <span id="node001">这是第1结点div001<span>

  <div id="002" style="border:1px solid red;width:80%;height:300px"> <span id="node002">这是第2结点div002<span>

    <div id="003" style="border:1px solid red;width:50%;height:200px"> <span id="node003">这是第3结点div003<span>

    </div>

    <div id="004" style="border:1px solid red;width:50%;height:200px"> <span id="node003">这是第3结点div004<span>

    </div>

  </div>

</div>

<script>...

function Get_srcElement()

...{

var srcElement=""

srcElement = srcElement + " " +  "event.srcElement.children[0].tagName: " + event.srcElement.children[0].tagName

srcElement = srcElement + " " +  "event.srcElement.firstChild.id : " + event.srcElement.firstChild.id

srcElement = srcElement + " " +  "event.srcElement.lastChild.id : " + event.srcElement.lastChild.id

alert(srcElement)

}

 

</script>

</div>

*****************************************************************************************************

<div style="border:10px red solid">

<input type="button" value="父元素标签" >

</div>

event.srcElement有关问题- -

 

event.srcElement的子标签 和父标签:

第一个子标签为

event.srcElement.firstChild

最后个一个是

event.srcElement.lastChild

当然也可以用

event.srcElement.children[i]

event.srcElement.ChildNode[i]

至于event.srcElement.parentElement是指在鼠标所在对象的上一个对象．

</span>

</body>

\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

<input type=button value=GO title="?page=1"

<script language="JavaScript">

function f()

{

  alert('index.asp'+event.srcElement.title)

  location.href='index.asp'+event.srcElement.title

}

</script>

<a title="a测试" >a测试</a>

<br>

<table border=1 width="200">

<tr title="tr测试" ><td>tr</td></tr>

</table>

<table border=1 width="200">

<tr><td title="td测试" >td</td></tr>

</table>

<select >

<option value="1">1</option>

<option value="2">2</option>

<option value="3">3</option>

<option value="4">4</option>

<option value="5">5</option>

</select>

 

javascript event.srcElement 兼容问题 
 

event = event? event: window.eventvar n = event.srcElement.id? event.srcElement.id:event.target.id; 第二句话firefox报错  event.srcElement未定义，怎么解决？ IE没问题
       
因为ff下本身不支持srcElement而是支持target,你这里这么用也是为了兼容浏览器,但是event.srcElement.id这么写会从event.srcElement里找id属性,这样是默认event.srcElement存在的,而火狐是不存在,当然就报错了.var obj=event.srcElement ? event.srcElement : event.target;再调用obj.id就行了.
 

点击整个页面除了id为 showMsg元素

    $(document).click(function() {
        //var event = window.event || arguments.callee.caller.arguments[0], srcElement = event.srcElement
                //|| event.target;]
        
       var evt = (evt) ? evt : ((window.event) ? window.event : "")
        if (!evt.srcElement.id && evt.srcElement.id!="showMsg")
        {
           $("#dvMenu").hide();
        }

 

 

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
    "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">  
 

 

<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
<head>
  <title></title>
  <script type="text/javascript">
    function tdclick(){
        if(event.srcElement.tagName.toLowerCase()=='td')
            alert("行："+(event.srcElement.parentNode.rowIndex+1)+"列："+(event.srcElement.cellIndex+1));
    }
  </script>
</head>
<body>
<table align="center" onclick="tdclick()" width="100%" height="400" cellspacing="1" border="1" bordercolor="#000000" bordercolorlight="#000000" bordercolordark="#C0C0C0" bgcolor="#C0C0C0">
  <tr>
    <td>&nbsp;</td>
    <td>&nbsp;</td>
    <td>&nbsp;</td>
    <td>&nbsp;</td>
  </tr>
  <tr>
    <td>&nbsp;</td>
    <td>&nbsp;</td>
    <td>&nbsp;</td>
    <td>&nbsp;</td>
  </tr>
  <tr>
    <td>&nbsp;</td>
    <td>&nbsp;</td>
    <td>&nbsp;</td>
    <td>&nbsp;</td>
  </tr>
</table>
</body>
</html>