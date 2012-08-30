<%@ page language="java" import="java.util.*" pageEncoding="GB18030"%> 
  
<%@ page import="java.sql.*;"%> 
  
<% 
  
    String path = request.getContextPath(); 
  
    String basePath = request.getScheme() + "://"
  
           + request.getServerName() + ":" + request.getServerPort() 
  
           + path + "/"; 
  
%> 
  
   
  
<%!boolean flag = false; 
  
    String str = "";%> 
  
   
  
<% 
  
    String login = (String) session.getAttribute("login"); 
  
    if (login != null && login.equals("true")) { 
  
       flag = true; 
  
    } 
  
%> 
  
   
  
<% 
  
    int pageSize = 3; //每页记录的条数 
  
    int pageNo = 0; //第几页 
  
    int tolPage = 0; //共几页 
  
    Class.forName("com.mysql.jdbc.Driver"); 
  
    String url = "jdbc:mysql://localhost/bbs?user=root&password=wangkang"; 
  
    Connection conn = DriverManager.getConnection(url); 
  
   
  
    Statement stmt = conn.createStatement(); 
  
    ResultSet rs = null; 
  
   
  
    //关于分页的算法--计算总页数 
  
    Statement stmt1 = conn.createStatement(); 
  
    rs = null; 
  
    rs = stmt1 
  
           .executeQuery("select count(*) from article where pid = 0"); 
  
    rs.next(); 
  
    int tolNum = rs.getInt(1); 
  
    stmt1.close(); 
  
      
  
     //计算总页数 
  
    if (tolNum % pageSize == 0) { 
  
       tolPage = tolNum / pageSize; 
  
    } else { 
  
       tolPage = tolNum / pageSize + 1; 
  
    } 
  
   
  
    /** 
  
     *分页算法 
  
     */
  
   
  
    String pageStr = request.getParameter("pageNo"); 
  
   
  
    if (pageStr == null || pageStr.equals("")) { 
  
       pageNo = 1; 
  
    } else { 
  
       if (Integer.parseInt(pageStr) <= 0) { 
  
           pageNo = 1; 
  
       } else if (Integer.parseInt(pageStr) >= tolPage) { 
  
           pageNo = tolPage; 
  
       } else { 
  
           pageNo = Integer.parseInt(pageStr); 
  
       } 
  
    } 
  
   
  
    int pagePos = (pageNo - 1) * pageSize; //起始记录位置 
  
   
  
    //Mysql中关于分页的sql语句 
  
    rs = stmt 
  
           .executeQuery("select * from article where pid=0 order by pdate desc limit "
  
                  + pagePos + "," + pageSize); 
  
    String strLogin = ""; 
  
    String fStr = ""; 
  
   
  
    while (rs.next()) { 
  
       if (flag) { 
  
           fStr = "<a href='Delete.jsp?pid=" + rs.getInt("pid") 
  
                  + "&id=" + rs.getInt("id") + "'>删除</a>"; 
  
       } 
  
   
  
       str += "<tr><td>" + rs.getInt("id") + "</td><td>"
  
              + "<a href='ShowDetail.jsp?id=" + rs.getInt("id") 
  
              + "'>" + rs.getString("title") + "</a>" + "</td><td>"
  
              + fStr + "</td></tr>"; 
  
    } 
  
   
  
    /* 
  
    //关于分页的算法--计算总页数 
  
    Statement stmt1 = conn.createStatement(); 
  
    rs = null; 
  
    rs = stmt1 
  
           .executeQuery("select count(*) from article where pid = 0"); 
  
    rs.next(); 
  
    int tolNum = rs.getInt(1); 
  
    stmt1.close(); 
  
   
  
    if (tolNum % 3 == 0) { 
  
       tolPage = tolNum / 3; 
  
    } else { 
  
       tolPage = tolNum / 3 + 1; 
  
    } 
  
     */ 
  
   
  
    /* 
  
    if (pageStr == null || pageStr.equals("")) { 
  
       pageNo = 1; 
  
    } else { 
  
       if (Integer.parseInt(pageStr) <= 0) { 
  
           pageNo = 1; 
  
       } else if(pageNo >= tolPage){ 
  
           pageNo = tolPage; 
  
       } else { 
  
           pageNo = Integer.parseInt(pageStr); 
  
       } 
  
    } 
  
     */
  
     
  
    rs.close(); 
  
    stmt.close(); 
  
    conn.close(); 
  
%> 
  
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"> 
  
<html> 
  
    <head> 
  
       <base href="<%=basePath%>"> 
  
   
  
       <title>My JSP 'ShowArticleTree.jsp' starting page</title> 
  
   
  
       <meta http-equiv="pragma" content="no-cache"> 
  
       <meta http-equiv="cache-control" content="no-cache"> 
  
       <meta http-equiv="expires" content="0"> 
  
       <meta http-equiv="keywords" content="keyword1,keyword2,keyword3"> 
  
       <meta http-equiv="description" content="This is my page"> 
  
       <!-- 
  
    <link rel="stylesheet" type="text/css" href="styles.css"> 
  
    --> 
  
   
  
    </head> 
  
   
  
    <body> 
  
       <a href="Post.jsp">增加新话题</a> 
  
       <table border="1"> 
  
           <%=str%> 
  
           <% 
  
              str = ""; 
  
              flag = false; 
  
           %> 
  
       </table> 
  
       共<%=tolPage%>页  第<%=pageNo%>页 
  
       <br> 
  
            
  
       <!-- 添加分页标记 --> 
  
        <!-- 分页第一种显示方法 --> 
  
       <a href="ShowArticleFlat.jsp?pageNo=<%=pageNo - 1%>"> < </a>   
  
       <a href="ShowArticleFlat.jsp?pageNo=<%=pageNo + 1%>"> > </a> 
  
       <br> 
  
       <br> 
  
         <!-- 分页第二种显示方法 --> 
  
       <form name="form1" action="ShowArticleFlat.jsp"> 
  
           <select name="pageNo" onchange="document.form1.submit()"> 
  
              <% 
  
                  for (int i = 1; i <= tolPage; i++) { 
  
              %> 
  
              <option value=<%=i%> <%=(pageNo == i) ? "selected" : ""%>> 
  
                  第<%=i%>页 
  
                  <% 
  
                  } 
  
              %> 
  
               
  
           </select> 
  
       </form> 
  
   
  
        <!-- 分页第三种显示方法 --> 
  
       <form name="form2" action="ShowArticleFlat.jsp"> 
  
           <input type="text" size=4 value=<%=pageNo%> name="pageNo"> 
  
           <input type="submit" value="GO"> 
  
       </form> 
  
    </body> 
  
</html>