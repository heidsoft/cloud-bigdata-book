JS获取父框架的内容:获取子框架的内容:js框架应用 
　　window.parent.id; ("id"为你要获取该值的名称); 
　　parent.document.getElementById("xxx"); 
　　parent.document.getElementsByTagName("body")[0]; 
　　获取子框架的内容: 
　　（第一个getElementById对象是子框架ID） 
　　 
　　document.getElementById("xxx").contentWindow.document.getElementById("xxxx"); 
　　 
　　document.getElementById("xxx").contentWindow.document.getElementsByTagName("body")[0]; 
　　 
　　-------------------------------------------- 
一.frame 

1．父框架到子框架的引用 
知道了上述原理，从父框架引用子框架变的非常容易，即： 
window.frames["frameName"]; 
这样就引用了页面内名为frameName的子框架。如果要引用子框架内的子框架，根据引用的框架实际就是window对象的性质，可以这样实现： 
window.frames["frameName"].frames["frameName2"]; 
这样就引用到了二级子框架，以此类推，可以实现多层框架的引用。 
2．子框架到父框架的引用 
每个window对象都有一个parent属性，表示它的父框架。如果该框架已经是顶层框架，则window.parent还表示该框架本身。 
3．兄弟框架间的引用 
如果两个框架同为一个框架的子框架，它们称为兄弟框架，可以通过父框架来实现互相引用，例如一个页面包括2个子框架： 
＜frameset rows="50%,50%"＞ 
＜frame src="1.html" name="frame1" /＞ 
＜frame src="2.html" name="frame2" /＞ 
＜/frameset＞ 
在frame1中可以使用如下语句来引用frame2： 
self.parent.frames["frame2"]; 
4．不同层次框架间的互相引用 
框架的层次是针对顶层框架而言的。当层次不同时，只要知道自己所在的层次以及另一个框架所在的层次和名字，利用框架引用的window对象性质，可以很容易地实现互相访问，例如： 
self.parent.frames["childName"].frames["targetFrameName"]; 
5．对顶层框架的引用 
和parent属性类似，window对象还有一个top属性。它表示对顶层框架的引用，这可以用来判断一个框架自身是否为顶层框架，例如： 
//判断本框架是否为顶层框架 
if(self==top){ 
//dosomething 
} 

改变框架的载入页面 
对框架的引用就是对window对象的引用，利用window对象的location属性，可以改变框架的导航，例如： 
window.frames[0].location＝"1.html"; 


引用其他框架内的JavaScript变量和函数 
在介绍引用其他框架内JavaScript变量和函数的技术之前，先来看以下代码： 
＜script language="JavaScript" type="text/javascript"＞ 

＜/script＞ 


二.iframe 

严格上应该是使用frames数组，和.document.all. 

＜iframe id=myiframe src="about:＜/iframe＞ 



三.href 

链接的 onclick 事件被先执行，其次是 href 属性下的动作（页面跳转，或 javascript 伪链接）； 
假设链接中同时存在 href 与 onclick，如果想让 href 属性下的动作不执行，onclick 必须得到一个 false 的返回值。不信，你可以将 goGoogle 函数中的 return false 注释掉； 
如果页面过长有滚动条，且希望通过链接的 onclick 事件执行操作。应将它的 href 属性设为 javascript:void(0);，而不要是 #，这可以防止不必要的页面跳动； 
如果在链接的 href 属性中调用一个有返回值的函数，当前页面的内容将被此函数的返回值代替； 
在按住Shift键的情况下会有所区别。 
今天我遇到的问题，在IE6.0里以href的形式访问不到parentNode。 
尽量不要用javascript:协议做为A的href属性，这样不仅会导致不必要的触发window.onbeforeunload事件，在IE里面更会使gif动画图片停止播放。 

