一、page 对象
    page对象代表JSP本身，更准确地说它代表JSP被转译后的Servlet，它可以调用Servlet类所定义的方法。
        
    二、config 对象
    config 对象里存放着一些Servlet 初始的数据结构。
    config 对象实现于javax.servlet.ServletConfig 接口，它共有下列四种方法：
        public String getInitParameter(name)
        public java.util.Enumeration getInitParameterNames( )
        public ServletContext getServletContext( )
        public Sring getServletName( ) 

    三、request 对象
    request 对象包含所有请求的信息，如：请求的来源、标头、cookies和请求相关的参数值等等。
    request 对象实现javax.servlet.http.HttpServletRequest接口的，所提供的方法可以将它分为四大类：
    1.储存和取得属性方法；
        void setAttribute(String name, Object value)    设定name属性的值为value
        Enumeration getAttributeNamesInScope(int scope)    取得所有scope 范围的属性
        Object getAttribute(String name)                取得name 属性的值
        void removeAttribute(String name)                移除name 属性的值
    2.取得请求参数的方法
        String getParameter(String name)                取得name 的参数值
        Enumeration getParameterNames( )                取得所有的参数名称
        String [] getParameterValues(String name)         取得所有name 的参数值
        Map getParameterMap( )                             取得一个要求参数的Map
    3.能够取得请求HTTP 标头的方法
        String getHeader(String name)                     取得name 的标头
        Enumeration getHeaderNames()                     取得所有的标头名称
        Enumeration getHeaders(String name)             取得所有name 的标头
        int getIntHeader(String name)                     取得整数类型name 的标头
        long getDateHeader(String name)                 取得日期类型name 的标头
        Cookie [] getCookies( )                         取得与请求有关的cookies
    4.其他的方法
        String getContextPath( )                         取得Context 路径(即站台名称)
        String getMethod( )                             取得HTTP 的方法(GET、POST)
        String getProtocol( )                             取得使用的协议 (HTTP/1.1、HTTP/1.0 )
        String getQueryString( )                        取得请求的参数字符串，不过，HTTP的方法必须为GET
        String getRequestedSessionId( )                 取得用户端的Session ID
        String getRequestURI( )                            取得请求的URL，但是不包括请求的参数字符串
        String getRemoteAddr( )                            取得用户的IP 地址
        String getRemoteHost( )                            取得用户的主机名称
        int getRemotePort( )                            取得用户的主机端口
        String getRemoteUser( )                         取得用户的名称
        void etCharacterEncoding(String    encoding)        设定编码格式，用来解决窗体传递中文的问题

    四、response 对象
    response 对象主要将JSP 处理数据后的结果传回到客户端。
    response 对象是实现javax.servlet.http.HttpServletResponse 接口。response对象所提供的方法。
    1.设定表头的方法
        void addCookie(Cookie cookie)                     新增cookie
        void addDateHeader(String name, long date)        新增long类型的值到name标头
        void addHeader(String name, String value)        新增String类型的值到name标头
        void addIntHeader(String name, int value)         新增int类型的值到name标头
        void setDateHeader(String name, long date)        指定long类型的值到name标头
        void setHeader(String name, String value)        指定String类型的值到name标头
        void setIntHeader(String name, int value)         指定int类型的值到name标头
    2.设定响应状态码的方法
        void sendError(int sc)                             传送状态码(status code)
        void sendError(int sc, String msg)                传送状态码和错误信息
        void setStatus(int sc)                             设定状态码
    3.用来URL 重写(rewriting)的方法    
        String encodeRedirectURL(String    url)            对使用sendRedirect( )方法的URL予以编码

    五、out 对象
    out 对象能把结果输出到网页上。
    out主要是用来控制管理输出的缓冲区(buffer)和输出流(output stream)。
        void clear( )                                     清除输出缓冲区的内容
        void clearBuffer( )                             清除输出缓冲区的内容
        void close( )                                     关闭输出流，清除所有的内容
        int getBufferSize( )                             取得目前缓冲区的大小(KB)
        int getRemaining( )                             取得目前使用后还剩下的缓冲区大小(KB)
        boolean isAutoFlush( )                            回传true表示缓冲区满时会自动清除；false表示不会自动清除并且产生异常处理
        
    六、session 对象
    session对象表示目前个别用户的会话(session)状况。
    session对象实现javax.servlet.http.HttpSession接口，HttpSession接口所提供的方法
        long getCreationTime()                            取得session产生的时间，单位是毫秒
        String getId()                                     取得session 的ID
        long getLastAccessedTime()                        取得用户最后通过这个session送出请求的时间
        long getMaxInactiveInterval()                    取得最大session不活动的时间，若超过这时间，session 将会失效
        void invalidate()                                取消session 对象，并将对象存放的内容完全抛弃
        boolean isNew()                                    判断session 是否为"新"的
        void setMaxInactiveInterval(int    interval)        设定最大session不活动的时间，若超过这时间，session 将会失效

    七、application对象
    application对象最常被使用在存取环境的信息。
    因为环境的信息通常都储存在ServletContext中，所以常利用application对象来存取ServletContext中的信息。
    application 对象实现javax.servlet.ServletContext 接口，ServletContext接口容器所提供的方法
        int getMajorVersion( )                             取得Container主要的Servlet API版本
        int getMinorVersion( )                             取得Container次要的Servlet API 版本
        String getServerInfo( )                         取得Container的名称和版本
        String getMimeType(String file)                 取得指定文件的MIME 类型
        ServletContext getContext(String uripath)        取得指定Local URL的Application context
        String getRealPath(String path)                 取得本地端path的绝对路径
        void log(String message)                         将信息写入log文件中
        void log(String message, Throwable throwable)    将stack trace 所产生的异常信息写入log文件中

    八、pageContext对象
    pageContext对象能够存取其他隐含对象。
    1.pageContext对象存取其他隐含对象属性的方法，此时需要指定范围的参数。
        Object getAttribute(String name, int scope)
        Enumeration getAttributeNamesInScope(int scope)
        void removeAttribute(String name, int scope)
        void setAttribute(String name, Object value, int scope)
    范围参数有四个，分别代表四种范围：PAGE_SCOPE、REQUEST_SCOPE、SESSION_SCOPE、APPLICATION_SCOPE
    2.PageContext对象取得其他隐含对象的方法
        Exception getException( )                        回传目前网页的异常，不过此网页要为error page，
        JspWriter getOut( )                             回传目前网页的输出流，例如：out 
        Object getPage( )                                回传目前网页的Servlet 实体(instance)，例如：page
        ServletRequest getRequest( )                    回传目前网页的请求，例如：request
        ServletResponse getResponse( )                    回传目前网页的响应，例如：response
        ServletConfig getServletConfig( )                回传目前此网页的ServletConfig 对象，例如：config
        ServletContext getServletContext( )                回传目前此网页的执行环境(context)，例如：application
        HttpSession getSession( )                        回传和目前网页有联系的会话(session)，例如：session
    3.PageContext对象提供取得属性的方法
        Object getAttribute(String name, int scope)        回传name 属性，范围为scope的属性对象，回传类型为Object
        Enumeration getAttributeNamesInScope(int scope)    回传所有属性范围为scope 的属性名称，回传类型为Enumeration
        int getAttributesScope(String name)             回传属性名称为name 的属性范围
        void removeAttribute(String name)                 移除属性名称为name 的属性对象
        void removeAttribute(String name, int scope)    移除属性名称为name，范围为scope 的属性对象
        void setAttribute(String name, Object value, int scope)        指定属性对象的名称为name、值为value、范围为scope
        Object findAttribute(String name)                寻找在所有范围中属性名称为name 的属性对象

    九、exception对象
    若要使用exception 对象时，必须在page 指令中设定。<%@ page isErrorPage="true" %>才能使用。
    exception提供的三个方法：
        getMessage( )
        getLocalizedMessage( )、
        printStackTrace(new java.io.PrintWriter(out))

 

 

EL表达式中的11个隐含对象

pageContext, pageScope, requestScope, sessionScope, applicationScope, param, paramValues, header, headerValues, cookies, initParam

这里的数据都是以map方式存放的，获取可用如：param.name, header.User_Agent, cookie.userinfo得到相应的数据。

${requestScope.origin_uri} 获取当前URI

 

 JSP的6个基本动作：

jsp:include         在页面被请求的时候引入一个文件

jsp:forward        把请求转到一个新的页面

jsp:plugin          根据浏览器类型为java插件生成object或embed标记

jsp:useBean     寻找或实例化一个JavaBean

jsp:getProperty    输出JavaBean的某个属性

jsp:setProperty     设置JavaBean的某个属性
