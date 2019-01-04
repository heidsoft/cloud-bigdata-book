最近工作中用到jQuery插件,需要安装eclipse插件才能支持jQuery智能提示,在网上搜索了一下,常用的有三个插件支持jQuery的智能提示:
1.jQueryWTP
2.Spket
3.Aptana 

 

在安装插件之前需要先安装eclipse,如果已经安装了eclipse,可以直接看下面的插件安装方法,本人使用的Fedora 12,操作如下:
1.直接打开”应用程序”-->”系统工具”-->”终端”
2.输入命令”su”,这个命令是临时切换到root用户
3.输入root用户的密码
4.输入命令”yum install eclipse”
5.安装成功后,就可以在”应用程序”-->”编程”-->”eclipse”,打开查看版本为eclipse 3.5.1,当然也可以直接下载安装,这里就不再细说了,安装完毕后就可以安装相关插件了.

 

以下分别介绍三个插件的安装方法:
一、jQueryWTP的安装方法(官方):
step1:download jqueryWTP_version.jar
step2:find you Eclipse Plugin org.eclipse.wst.javascript.ui_xxxxxxx.jar,backup the plugin.
step3:double click the jar file or run with command java -jar jqueryWTP.version.jar
step4:on the opened swing UI,choose org.eclipse.wst.javascript.ui_xxxxxxx.jar,and output dir.
step5:click generate button.
step6:replace old org.eclipse.wst.javascript.ui_xxxxxxx.jar file with the generated file.
step7:restart eclipse.
step8:open a html file,edit js content. 

jQueryWTP插件项目的主页:http://www.langtags.com/jquerywtp/
但是下载完jqueryWTP0.31foCN.jar后,根据安装教程里都提到了一个plugin目录下的org.eclipse.wst.javascript.ui_xxxxxxx.jar文件，但是我的eclipse目录想压根就找不到这个文件,所以也要就不能使用jQueryWTP插件智能提示.

后来仔细查看jQueryWTP插件项目的主页,有这么一句话,Eclipse的WTP支持Javascript的代码补全功能,但是很简单，而且不支持jQuery,jQueryWTP的目的就是让Eclipse WTP支持jQuery,需要说明的是该插件对于MyEclipse等基于Eclipse WTP的工具也是支持的.
根据这句话可以猜想找不到plugin目录下的org.eclipse.wst.javascript.ui_xxxxxxx.jar文件的原因,可能是没有安装eclipse WTP插件,由于下面两种方法顺利安装并且支持jQuery智能提示,所以这里的猜想还没有经过验证.

 

二、Spket的安装方法（两种方式）： 
1.在线安装：Help->Install New Software...->Add...->Name: "Spket",
Location:http://www.spket.com/update/ 下载完毕重启Eclipse.
2.手动安装：到http://www.spket.com/download.html下载 Plugin 版本，当前版本为1.6.17.下载解压后直接放置于Eclipse的dropins目录下，重启Eclipse. 
3.下载jQuery文件，（要下载开发版本）. 
4.设置spket ,Window -> Preferences -> Spket -> JavaScript Profiles -> New,输入“jQuery”点击OK； 选择“jQuery” 并点击“Add Library”然后在下拉条中选取“jQuery”； 选择 “jQuery”并点击“Add File”，然后选中你下载的jQuery.js 文件；设成Default; 
5.设置js打开方式(这一步很重要,不设置的话,也不会有jQuery的智能提示), Window -> Preferences ->General-> Editors-> File Associations-> 选择*.js,将Spket JavaScript Editor设为Default。 
6.新建一个js文件，jQuery自动提示！！

 

三、Aptana的安装方法:
Aptana的安装相对比较简单,直接安装完毕就支持jQuery的智能提示,而且智能提示功能也比较强大,按照下面的安装就能使用,强烈建议初学者可以使用这种方法安装:
1.在线安装：Help->Install New Software...->Add...->Name: "Aptana",
Location:http://download.aptana.com/tools/studio/plugin/install/studio下载完毕重启Eclipse,Aptana插件安装成功,支持javascript智能提示功能,但还不支持jQuery智能提示,需要再安装支持jQuery智能提示的插件.
2.Window->My Studio打开Aptana的首页,单击首页上面的Plugins,选择Ajax下面的jQuery Support,单击Get It即可安装jQuery智能提示的插件
