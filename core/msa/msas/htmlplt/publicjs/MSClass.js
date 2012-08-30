<!--
/*MSClass (Class Of Marquee Scroll通用不间断滚动JS封装类) Ver 1.6*\

　制作时间:2006-08-29 (Ver 0.5)
　发布时间:2006-08-31 (Ver 0.8)
　更新时间:2007-01-31 (Ver 1.6)
　更新说明: + 加入功能 * 修正、完善
	1.6.070131
		+ 禁止鼠标控制暂停或继续 (将第9个参数设置为-1或者动态赋值将ScrollSetp设置为-1)
		+ 判断是否可以滚动 (若内容区域小于显示区域,则自动取消滚动)
		+ 跳过初始化错误 (避免引起其它滚动的停止)
		+ 默认值 (除容器ID必选外，其他参数均可根据情况进行选择设置)
		+ 参数动态赋值 (方向可用英文表示top|bottom|left|right,使其更直观、方便)
		* 文字滚动不准确 (本次更新主要目的解决此Bug,感谢周珺参与测试)
	1.4.061211
		+ 鼠标悬停改变滚动方向 (鼠标悬停控制左右滚动)
		* 由于文档下载过慢而导致获取的高度/宽度不准确
		* 浏览器兼容问题 (IE、FF、Opera、NS、MYIE)
	1.2.060922
		+ 指定范围间歇滚动
		* 程序调整
		* 连续间歇滚动停止的错误
	1.0.060901
		+ 向下、向右滚动
		+ 开始等待时间
		+ 连续滚动
		* 调整时间单位
		* 滚动误差
		* 随机死循环
		* 加强性能
		* 程序优化
	0.8.060829
		  翻屏不间断向上、向左滚动

　应用说明:页面包含<script type="text/javascript" src="MSClass.js"></script>
	
	创建实例:
		//参数直接赋值法
		new Marquee("marquee")
		new Marquee("marquee","top")
		......
		new Marquee("marquee",0,1,760,52)
		new Marquee("marquee","top",1,760,52,50,5000)
		......
		new Marquee("marquee",0,1,760,104,50,5000,3000,52)
		new Marquee("marquee",null,null,760,104,null,5000,null,-1)

		//参数动态赋值法
		var marquee1 = new Marquee("marquee")	*此参数必选
		marquee1.Direction = "top";	或者	marquee1.Direction = 0;
		marquee1.Step = 1;
		marquee1.Width = 760;
		marquee1.Height = 52;
		marquee1.Timer = 50;
		marquee1.DelayTime = 5000;
		marquee1.WaitTime = 3000;
		marquee1.ScrollStep = 52;
		marquee1.Start();

	参数说明:
		ID		"marquee"	容器ID		(必选)
		Direction	(0)		滚动方向	(可选,默认为0向上滚动) 可设置的值包括:0,1,2,3,"top","bottom","left","right" (0向上 1向下 2向左 3向右)
		Step		(1)		滚动的步长	(可选,默认值为2,数值越大,滚动越快)
		Width		(760)		容器可视宽度	(可选,默认值为容器初始设置的宽度)
		Height		(52)		容器可视高度	(可选,默认值为容器初始设置的高度)
		Timer		(50)		定时器		(可选,默认值为30,数值越小,滚动的速度越快,1000=1秒,建议不小于20)
		DelayTime	(5000)		间歇停顿延迟时间(可选,默认为0不停顿,1000=1秒)
		WaitTime	(3000)		开始时的等待时间(可选,默认或0为不等待,1000=1秒)
		ScrollStep	(52)		间歇滚动间距	(可选,默认为翻屏宽/高度,该数值与延迟均为0则为鼠标悬停控制,-1禁止鼠标控制)
　使用建议:
		1、建议直接赋予容器的显示区域的宽度和高度，如(<div id="marquee" style="width:760px;height:52px;">......</div>)
		2、建议为容器添加样式overflow = auto，如(<div id="marquee" style="width:760px;height:52px;overflow:auto;">......</div>)
		3、为了更准确的获取滚动区域的宽度和高度，请尽可能将各滚动单位直接赋予正确宽高度
		4、对于TABLE标记的横向滚动，需要对TABLE添加样式display = inline，如(<div id="marquee" style="width:760px;height:52px;overflow:auto;"><table style="display:inline">......</table></div>)
		5、对于翻屏滚动或间歇滚动，要注意各滚动单位间的间距，同时需要对容器的可视高度和可视宽度做好准确的设置，对于各滚动单位间的间距可以通过设置行间距或者单元格的高宽度来进行调整
		6、对于LI自动换行的问题暂时没有更好的解决办法，建议将其转换成表格(TABLE)的形式来达到同等的效果
		7、针对横向滚动的文字段落，如果最末端是以空格" "结束的，请将空格" "转换成"&nbsp;"
		8、鼠标悬停滚动思想源自Flash，所以有一定的局限性（容器内仅允许用图片<img>或者带链接的图片<a><img></a>的形式，并需要禁止其自动换行）

\***/


function Marquee()
{
	this.ID = document.getElementById(arguments[0]);
	if(!this.ID)
	{
		alert("ID:\"" + arguments[0] + "\"ERROR!");
		this.ID = -1;
		return;
	}
	this.Direction = this.Width = this.Height = this.DelayTime = this.WaitTime = this.Correct = this.CTL = this.StartID = this.Stop = this.MouseOver = 0;
	this.Step = 1;
	this.Timer = 30;
	this.DirectionArray = {"top":0 , "bottom":1 , "left":2 , "right":3};
	if(typeof arguments[1] == "number" || typeof arguments[1] == "string")this.Direction = arguments[1];
	if(typeof arguments[2] == "number")this.Step = arguments[2];
	if(typeof arguments[3] == "number")this.Width = arguments[3];
	if(typeof arguments[4] == "number")this.Height = arguments[4];
	if(typeof arguments[5] == "number")this.Timer = arguments[5];
	if(typeof arguments[6] == "number")this.DelayTime = arguments[6];
	if(typeof arguments[7] == "number")this.WaitTime = arguments[7];
	if(typeof arguments[8] == "number")this.ScrollStep = arguments[8]
	this.ID.style.overflow = this.ID.style.overflowX = this.ID.style.overflowY = "hidden";
	this.ID.noWrap = true;
	this.IsNotOpera = (navigator.userAgent.toLowerCase().indexOf("opera") == -1);
	if(arguments.length >= 7)this.Start();
}


Marquee.prototype.Start = function()
{
	if(this.ID == -1)return;
	if(this.WaitTime < 800)this.WaitTime = 800;
	if(this.Timer < 20)this.Timer = 20;
	if(this.Width == 0)this.Width = parseInt(this.ID.style.width);
	if(this.Height == 0)this.Height = parseInt(this.ID.style.height);
	if(typeof this.Direction == "string")this.Direction = this.DirectionArray[this.Direction.toString().toLowerCase()];
	this.HalfWidth = Math.round(this.Width / 2);
	this.HalfHeight = Math.round(this.Height / 2);
	this.BakStep = this.Step;
	this.ID.style.width = this.Width + "px";
	this.ID.style.height = this.Height + "px";
	if(typeof this.ScrollStep != "number")this.ScrollStep = this.Direction > 1 ? this.Width : this.Height;
	var msobj = this;
	var timer = this.Timer;
	var delaytime = this.DelayTime;
	var waittime = this.WaitTime;
	msobj.StartID = function(){msobj.Scroll()}
	msobj.Continue = function()
				{
					if(msobj.MouseOver == 1)
					{
						setTimeout(msobj.Continue,delaytime);
					}
					else
					{	clearInterval(msobj.TimerID);
						msobj.CTL = msobj.Stop = 0;
						msobj.TimerID = setInterval(msobj.StartID,timer);
					}
				}

	msobj.Pause = function()
			{
				msobj.Stop = 1;
				clearInterval(msobj.TimerID);
				setTimeout(msobj.Continue,delaytime);
			}

	msobj.Begin = function()
		{
			msobj.ClientScroll = msobj.Direction > 1 ? msobj.ID.scrollWidth : msobj.ID.scrollHeight;
			if((msobj.Direction <= 1 && msobj.ClientScroll <= msobj.Height + msobj.Step) || (msobj.Direction > 1 && msobj.ClientScroll <= msobj.Width + msobj.Step))return;
			msobj.ID.innerHTML += msobj.ID.innerHTML;
			msobj.TimerID = setInterval(msobj.StartID,timer);
			if(msobj.ScrollStep < 0)return;
			msobj.ID.onmousemove = function(event)
						{
							if(msobj.ScrollStep == 0 && msobj.Direction > 1)
							{
								var event = event || window.event;
								if(window.event)
								{
									if(msobj.IsNotOpera)
									{
										msobj.EventLeft = event.srcElement.id == msobj.ID.id ? event.offsetX - msobj.ID.scrollLeft : event.srcElement.offsetLeft - msobj.ID.scrollLeft + event.offsetX;
									}
									else
									{
										msobj.ScrollStep = null;
										return;
									}
								}
								else
								{
									msobj.EventLeft = event.layerX - msobj.ID.scrollLeft;
								}
								msobj.Direction = msobj.EventLeft > msobj.HalfWidth ? 3 : 2;
								msobj.AbsCenter = Math.abs(msobj.HalfWidth - msobj.EventLeft);
								msobj.Step = Math.round(msobj.AbsCenter * (msobj.BakStep*2) / msobj.HalfWidth);
							}
						}
			msobj.ID.onmouseover = function()
						{
							if(msobj.ScrollStep == 0)return;
							msobj.MouseOver = 1;
							clearInterval(msobj.TimerID);
						}
			msobj.ID.onmouseout = function()
						{
							if(msobj.ScrollStep == 0)
							{
								if(msobj.Step == 0)msobj.Step = 1;
								return;
							}
							msobj.MouseOver = 0;
							if(msobj.Stop == 0)
							{
								clearInterval(msobj.TimerID);
								msobj.TimerID = setInterval(msobj.StartID,timer);
							}
						}
		}
	setTimeout(msobj.Begin,waittime);
}

Marquee.prototype.Scroll = function()
{
	switch(this.Direction)
	{
		case 0:
			this.CTL += this.Step;
			if(this.CTL >= this.ScrollStep && this.DelayTime > 0)
			{
				this.ID.scrollTop += this.ScrollStep + this.Step - this.CTL;
				this.Pause();
				return;
			}
			else
			{
				if(this.ID.scrollTop >= this.ClientScroll)
				{
					this.ID.scrollTop -= this.ClientScroll;
				}
				this.ID.scrollTop += this.Step;
			}
		break;

		case 1:
			this.CTL += this.Step;
			if(this.CTL >= this.ScrollStep && this.DelayTime > 0)
			{
				this.ID.scrollTop -= this.ScrollStep + this.Step - this.CTL;
				this.Pause();
				return;
			}
			else
			{
				if(this.ID.scrollTop <= 0)
				{
					this.ID.scrollTop += this.ClientScroll;
				}
				this.ID.scrollTop -= this.Step;
			}
		break;

		case 2:
			this.CTL += this.Step;
			if(this.CTL >= this.ScrollStep && this.DelayTime > 0)
			{
				this.ID.scrollLeft += this.ScrollStep + this.Step - this.CTL;
				this.Pause();
				return;
			}
			else
			{
				if(this.ID.scrollLeft >= this.ClientScroll)
				{
					this.ID.scrollLeft -= this.ClientScroll;
				}
				this.ID.scrollLeft += this.Step;
			}
		break;

		case 3:
			this.CTL += this.Step;
			if(this.CTL >= this.ScrollStep && this.DelayTime > 0)
			{
				this.ID.scrollLeft -= this.ScrollStep + this.Step - this.CTL;
				this.Pause();
				return;
			}
			else
			{
				if(this.ID.scrollLeft <= 0)
				{
					this.ID.scrollLeft += this.ClientScroll;
				}
				this.ID.scrollLeft -= this.Step;
			}
		break;
	}
}
//-->