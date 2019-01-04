lighttpd shttpd thttpd boa mathopd minihttpd 

编写一个TCP服务器
 用户用浏览器访问这个TCP服务器进行交互
 
软件流程：
 1、接收客户端连接
 2、接收HTTP请求
 3、解析HTTP请求确定所需要执行的功能以及相关参数
 4、执行功能（调用函数）
 5、根据执行结果构造HTTP响应（页面信息）
 6、发送HTTP响应到客户端
 7、断开连接
 
以APACHE + PHP为例
 步骤1和2是APACHE做的
 步骤3是PHP做的
 步骤4是PHP调用外部命令或函数做的
 步骤5也是PHP做的
 步骤6和7是APACHE做的
 