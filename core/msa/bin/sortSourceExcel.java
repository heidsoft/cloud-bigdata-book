import java.util.Calendar;
import org.apache.poi.ss.usermodel.CellStyle;
import org.apache.poi.ss.util.Region;
import org.apache.poi.ss.usermodel.Font;
import org.apache.poi.ss.usermodel.IndexedColors;
import org.apache.poi.hssf.record.cf.BorderFormatting;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
public class sortSourceExcel{
	public static void main(String args[]) throws IOException{
			String source=args[0];
	    String destination=args[1];
	    String titlestr="";
	    String column1="";
	    String column2="";
	    String column3="";
	    String column4="";
	    String column5="";
	    String column6="";
	    int type=Integer.parseInt(args[2]);
	    System.out.println(type);
	    if(type==1){
	    	titlestr="部门流量排序";
	    	column1="部门名称";
	    	column1="用户名";
	    	column1="日期";
	    	column1="服务";
	    	column1="流量";
	    	column1="连接时长";
	  	}else if(type==2){
	  		titlestr="人员流量排序";
	    	column1="用户名";
	    	column1="部门名称";
	    	column1="日期";
	    	column1="服务";
	    	column1="流量";
	    	column1="连接时长";
	  	}if(type==3){
	  		titlestr="服务流量排序";
	    	column1="用户名";
	    	column1="服务名称";
	    	column1="部门名称";
	    	column1="流量";
	    	column1="日期";
	    	column1="连接时长";
	    }if(type==5){
	    	titlestr="日期流量排序";
	    	column1="日期";
	    	column1="用户名";
	    	column1="部门";
	    	column1="服务";
	    	column1="流量";
	    	column1="连接时长";
	    }if(type==7){
	    	titlestr="网址访问排序";
	    	column1="用户名";
	    	column1="网址";
	    	column1="部门名称";
	    	column1="流量";
	    	column1="点击次数";
	    	column1="连接时间";
	    }
		XLSCreate e=new XLSCreate(destination);
        //字体
		    Font headfont = e.workbook.createFont();
		    headfont.setFontName("黑体");
		    headfont.setFontHeightInPoints((short) 22);// 字体大小
		    headfont.setBoldweight(Font.BOLDWEIGHT_BOLD);// 加粗
		    //标题
		    CellStyle headstyle = e.workbook.createCellStyle();
		    headstyle.setFont(headfont);
		    headstyle.setAlignment(CellStyle.ALIGN_CENTER);// 左右居中
		    headstyle.setVerticalAlignment(CellStyle.VERTICAL_CENTER);// 上下居中
		    headstyle.setLocked(true);
		    headstyle.setWrapText(true);// 自动换行
        //一个样式
		    CellStyle title = e.workbook.createCellStyle();
		    title.setBorderBottom(BorderFormatting.BORDER_THIN); // 设置单元格的边框为粗体
		    title.setBottomBorderColor(IndexedColors.BLACK.getIndex()); // 设置单元格的边框颜色．
		    title.setBorderTop(BorderFormatting.BORDER_THIN); // 设置单元格的边框为粗体
		    title.setTopBorderColor(IndexedColors.BLACK.getIndex()); // 设置单元格的边框颜色．
		    title.setBorderLeft(BorderFormatting.BORDER_THIN); // 设置单元格的边框为粗体
		    title.setLeftBorderColor(IndexedColors.BLACK.getIndex()); // 设置单元格的边框颜色．
		    title.setBorderRight(BorderFormatting.BORDER_THIN); // 设置单元格的边框为粗体
		    title.setRightBorderColor(IndexedColors.BLACK.getIndex()); // 设置单元格的边框颜色

				//字体
		    Font columnHeadFont = e.workbook.createFont();
		    columnHeadFont.setFontName("宋体");
		    columnHeadFont.setFontHeightInPoints((short) 10);
		    columnHeadFont.setBoldweight(Font.BOLDWEIGHT_BOLD);
		    // 列头的样式
		    CellStyle columnHeadStyle = e.workbook.createCellStyle();
		    columnHeadStyle.setFont(columnHeadFont);
		    columnHeadStyle.setAlignment(CellStyle.ALIGN_LEFT);// 左右居中
		    columnHeadStyle.setVerticalAlignment(CellStyle.VERTICAL_BOTTOM);// 上下居中
		    columnHeadStyle.setLocked(true);
		    columnHeadStyle.setWrapText(true);

		    columnHeadStyle.setBorderBottom(BorderFormatting.BORDER_THIN); // 设置单元格的边框为粗体
		    columnHeadStyle.setBorderTop(BorderFormatting.BORDER_THIN); // 设置单元格的边框为粗体
		    columnHeadStyle.setBorderLeft(BorderFormatting.BORDER_THIN); // 设置单元格的边框为粗体．
		    columnHeadStyle.setBorderRight(BorderFormatting.BORDER_THIN); // 设置单元格的边框为粗体
		    //小标题
		    CellStyle titleStyle = e.workbook.createCellStyle();
		    titleStyle.setFont(columnHeadFont);
		    titleStyle.setAlignment(CellStyle.ALIGN_CENTER);// 左右居中
		    titleStyle.setVerticalAlignment(CellStyle.VERTICAL_CENTER);// 上下居中
		    titleStyle.setLocked(true);
		    titleStyle.setWrapText(true);

		    titleStyle.setBorderBottom(BorderFormatting.BORDER_THIN); // 设置单元格的边框为粗体
		    titleStyle.setBorderTop(BorderFormatting.BORDER_THIN); // 设置单元格的边框为粗体
		    titleStyle.setBorderLeft(BorderFormatting.BORDER_THIN); // 设置单元格的边框为粗体．
		    titleStyle.setBorderRight(BorderFormatting.BORDER_THIN); // 设置单元格的边框为粗体
				// 普通单元格样式
			Font font = e.workbook.createFont();
		    font.setFontName("宋体");
		    font.setFontHeightInPoints((short) 10);
		    CellStyle style = e.workbook.createCellStyle();
		    style.setFont(font);
		    style.setAlignment(CellStyle.ALIGN_LEFT);// 左右居中
		    style.setVerticalAlignment(CellStyle.VERTICAL_BOTTOM);// 上下居中
		    style.setWrapText(true);
		    style.setLeftBorderColor(IndexedColors.BLACK.getIndex());
		    style.setBorderLeft((short) 1);
		    style.setRightBorderColor(IndexedColors.BLACK.getIndex());
		    style.setBorderRight((short)1);
		    style.setBorderLeft(BorderFormatting.BORDER_THIN); // 设置单元格的边框为粗体
		    style.setBorderRight(BorderFormatting.BORDER_THIN); // 设置单元格的边框为粗体
		    style.setBottomBorderColor(IndexedColors.BLACK.getIndex()); // 设置单元格的边框颜色
		    style.setFillForegroundColor(IndexedColors.WHITE.getIndex());// 设置单元格的背景颜色

	        e.createRow(0,500);//第一行，并设置高度
	        e.mergingCell(0,0,0,5);
	        Region region = new Region((short)0,(short)0,(short)0,(short)5);
	        e.setRegionStyle(region,title);
	        e.setCell(0,titlestr,headstyle);

	        e.createRow(1);
        	e.mergingCell(1,1,1,5);
        	Region region1 = new Region((short)1,(short)0,(short)1,(short)5);
        	e.setRegionStyle(region1,title);
        	e.setCell(0,"报表名称",columnHeadStyle);
        	e.setCell(1,titlestr,style);

	        e.createRow(2);
        	e.setCell(0,column1,columnHeadStyle,100);
        	e.setCell(1,column2,columnHeadStyle,100);
        	e.setCell(2,column3,columnHeadStyle,100);
        	e.setCell(3,column4,columnHeadStyle,100);
        	e.setCell(4,column5,columnHeadStyle,100);
        	e.setCell(5,column6,columnHeadStyle,100);

	        int rownum=0;
	        BufferedReader reader = null;
	        String line = null;
		    	reader = new BufferedReader(new FileReader(source));
		    	while((line=reader.readLine())!=null){
		    	   String item[] = line.split(",");//将字符串分隔成字符数组
		    	   e.createRow(rownum+3,230);
					   //e.setCell(0,String.valueOf(rownum+1),style);
					   //System.out.println(item.length);
		    	   for(int j=0;j<item.length;j++){
					      	e.setCell(j,item[j],style);
						 }
					 rownum++;
					 if(rownum==30000) break;
		   		}
			    try {
						e.exportXLS();
				  } catch (Exception e1) {
						e1.printStackTrace();
				  }
   }
}