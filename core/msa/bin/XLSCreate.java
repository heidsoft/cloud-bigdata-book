import java.io.*;
import org.apache.poi.hssf.usermodel.HSSFWorkbook;
import org.apache.poi.hssf.usermodel.HSSFSheet;
import org.apache.poi.hssf.usermodel.HSSFRow;
import org.apache.poi.hssf.usermodel.HSSFCell;
import org.apache.poi.ss.util.Region;
import org.apache.poi.hssf.util.HSSFCellUtil;
import org.apache.poi.ss.usermodel.CellStyle;
import org.apache.poi.hssf.extractor.ExcelExtractor;
import org.apache.poi.poifs.filesystem.POIFSFileSystem;
import org.apache.poi.hssf.util.CellRangeAddress;
import java.io.FileInputStream;
import java.util.Calendar;

public class XLSCreate {
	  //  设置cell编码解决中文高位字节截断
    //private   static   short  XLS_ENCODING=HSSFWorkbook.ENCODING_UTF_8;
    //  定制日期格式
    //private   static  String DATE_FORMAT  =   " m/d/yy " ;  //  "m/d/yy h:mm"
    //  定制浮点数格式
    //private   static  String NUMBER_FORMAT  =   " #,##0.00 " ;
		private  String xlsFileName;
    public  HSSFWorkbook workbook;
    private  HSSFSheet sheet;
    public  HSSFRow row;
    public  XLSCreate(String fileName){
         this.xlsFileName=fileName;
         this.workbook=new  HSSFWorkbook();
         this.sheet=workbook.createSheet("sheet1");
    }
    public  void  exportXLS() throws Exception{
            try{
	            FileOutputStream fOut=new  FileOutputStream(xlsFileName);
	            workbook.write(fOut);
	            fOut.flush();
	            fOut.close();
		        }catch  (FileNotFoundException e){
             System.out.println("文件不存在");
		        }catch  (IOException e){
             System.out.println("输入错误");
		        }
    }
    /** */ /**
    * 增加一行
    *
    *  @param  index
    *            行号
    */
    public   void  createRow( int  index){
         this.row=this.sheet.createRow(index);
    }
    /** */ /**
    * 增加一行
    *
    *  @param  index
    *            行号
    *  @param  height
    *            行高
    */
    public   void  createRow(int index,int height){
         this.row=this.sheet.createRow(index);
         row.setHeight((short)height);
    }
    /** */ /**
    * 设置单元格
    *
    *  @param  index
    *            列号
    *  @param  value
    *            单元格填充值
    */
    public   void  setCell( int index,String value){
      HSSFCell cell=this.row.createCell(( short ) index);
      cell.setCellType(HSSFCell.CELL_TYPE_STRING);
      cell.setCellValue(value);
    }
    /** */ /**
    * 设置单元格
    *
    *  @param  index
    *            列号
    *  @param  value
    *            单元格填充值
    *  @param  style
    *            样式
    */
    public   void  setCell( int index,String value,CellStyle style){
      HSSFCell cell=this.row.createCell(( short ) index);
      cell.setCellType(HSSFCell.CELL_TYPE_STRING);
      cell.setCellValue(value);
      cell.setCellStyle(style);
    }
    /** */ /**
    * 设置单元格
    *
    *  @param  index
    *            列号
    *  @param  value
    *            单元格填充值
    *  @param  style
    *            样式
    *  @param  width
    *            宽度
    */
    public void  setCell( int index,String value,CellStyle style,int width){
      HSSFCell cell=this.row.createCell(( short ) index);
      this.sheet.setColumnWidth((short)index,(short)width*36);
      cell.setCellType(HSSFCell.CELL_TYPE_STRING);
      //cell.setEncoding(XLS_ENCODING);
      cell.setCellValue(value);
      cell.setCellStyle(style);
    }
    /**
     * 合并单元格加边框  水平
     * @param sheet
     * @param region
     * @param cs
     */
    public void setRegionStyle(Region region,CellStyle cs){
	         int toprowNum = region.getRowFrom();
	         for(int i=region.getRowFrom();i <= region.getRowTo(); i ++) {
	             HSSFRow row = HSSFCellUtil.getRow(i,this.sheet);
	             for (int j = region.getColumnFrom(); j <= region.getColumnTo(); j++) {
	                 HSSFCell cell = HSSFCellUtil.getCell(row,(short)j);
	                 cell.setCellStyle(cs);
	             }
	         }
	  }
    /** */ /**
    * 合并单元格
    *
    *  @param  rowFrom
    *            行开始
    *  @param  rowTo
    *            行结束
    *  @param  columnFrom
    *            列开始
    *  @param  columnTo
    *            列结束
    */
    public   void  mergingCell( int rowFrom,int rowTo,int columnFrom,int columnTo){
      this.sheet.addMergedRegion(new CellRangeAddress(rowFrom,rowTo,columnFrom,columnTo));
    }
}
