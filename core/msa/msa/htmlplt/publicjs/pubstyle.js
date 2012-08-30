//—————————–初始化主题—————————————————————-   
//定义skin  array二维数给 
var skin_Array= Array_2(4,3); 
//初始化数组 index name cssUrl  列4为没有选中的图标  列5为选中时的图标 
//索引为0的主题为默认主题 
skin_Array[0][0]="0"; 
skin_Array[0][1]="经典"; 
skin_Array[0][2]="/ext/resources/css/xtheme-default.css"; 

skin_Array[1][0]="1"; 
skin_Array[1][1]="深蓝"; 
skin_Array[1][2]="/ext/resources/css/xtheme-blueen.css"; 


skin_Array[2][0]="2"; 
skin_Array[2][1]="浅灰"; 
skin_Array[2][2]="/ext/resources/css/xtheme-gray.css"; 



skin_Array[3][0]="3"; 
skin_Array[3][1]="金黄"; 
skin_Array[3][2]="/ext/resources/css/xtheme-human.css" ; 


//—————————–全局主题配置js, 需要调用主题的直接引用此js—- 

initTheme(); 

//以下是生成二级数组的函数 
function Array_2(nRow,nColumn){ 
    var array1=new Array(); //定义一维数组 
    for(i=0;i<nRow;i++) 
    { 
        array1[i]=new Array(); //将每一个子元素又定义为数组 
        for(n=0;n<nColumn;n++) 
        { 
            array1[i][n]=''; //此时aa[i][n]可以看作是一个二级数组 
        } 
    } 
    return array1; 
} 

function initTheme() 
{   
     var theme_index= getCookie("themeindex"); 
    //var theme_url=""; 
    //根据主题名解析出主题使用的主题 
     if(theme_index != ""){ 
                 Ext.util.CSS.swapStyleSheet('theme',skin_Array[theme_index][2]); 
     } 
     else  //如果没有选择主题，加载默认主题 
     { 
                Ext.util.CSS.swapStyleSheet('theme',skin_Array[0][2]); 
     } 
} 
//—————————-以上为取得cookie 中保存的主题并设置应用——————— 

//———————————————————-设置主题函数start——————————————————-    
function setTheme(theme_index) 
{ 
  //设置应用主题 
    Ext.util.CSS.swapStyleSheet('theme',skin_Array[theme_index][2]);    
    setCookie("themeindex",theme_index); 
} 

//—————————————————–设置主题结束—————————————————————- 

//————————————-设置cookie——————————————————— 
function setCookie(name, value) { 
        var today = new Date(); 
        var expires = new Date(); 
        expires.setTime(today.getTime() + 1000 * 60 * 60 * 24 * 365);   // 保存一年时间的cookie 
        document.cookie = name + "=" + escape(value) + "; expires=" + expires.toGMTString(); 
        } 

//————————————-获取cookie——————————————————— 
function getCookie(Name) { 
        var search = Name + "="; 
        if (document.cookie.length > 0) { 
            offset = document.cookie.indexOf(search); 
            if (offset != -1) { 
             offset += search.length; 
             end = document.cookie.indexOf(";", offset); 
             if (end == -1) { 
              end = document.cookie.length; 
             } 
             return unescape(document.cookie.substring(offset, end)); 
            } else { 
             return (""); 
            } 
        } else { 
            return (""); 
        } 
}
