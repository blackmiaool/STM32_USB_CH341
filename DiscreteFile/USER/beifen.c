#include "lcd.h"
#include "stdlib.h"
#include "font.h" 
#include "usart.h"
#include "delay.h"	 
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//Mini STM32开发板
//2.4/2.8寸TFT液晶驱动	  
//支持驱动IC型号包括:ILI9325/RM68021/ILI9320/ILI9328/LGDP4531/LGDP4535/SPFD5408/SSD1289/1505/B505等
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2011/1/13
//版本：V1.6
//版权所有，盗版必究。
//Copyright(C) 正点原子 2009-2019
//All rights reserved
//********************************************************************************
//V1.2修改说明
//支持了SPFD5408的驱动,另外把液晶ID直接打印成HEX格式.方便查看LCD驱动IC.
//V1.3
//加入了快速IO的支持
//修改了背光控制的极性（适用于V1.8及以后的开发板版本）
//对于1.8版本之前(不包括1.8)的液晶模块,请修改LCD_Init函数的LCD_LED=1;为LCD_LED=1;
//V1.4
//修改了LCD_ShowChar函数，使用画点功能画字符。
//加入了横竖屏显示的支持
//V1.5 20110730
//1,修改了B505液晶读颜色有误的bug.
//2,修改了快速IO及横竖屏的设置方式.
//V1.6 20111116
//1,加入对LGDP4535液晶的驱动支持
//////////////////////////////////////////////////////////////////////////////////
extern u16 fanse[];
u8 zifubiaozhi;	   
u8 xx,yy;
s8 deltaxx,deltayy;					 
//画笔颜色,背景颜色
u16 POINT_COLOR = 0x0000,BACK_COLOR = 0xFFFF;  
u16 DeviceCode;	 
u8 jishu,biaozhi;
#if LCD_FAST_IO==1 //快速IO
//写寄存器函数
void LCD_WR_REG(u8 data)
{ 
	LCD_RS_CLR;//写地址  
 	LCD_CS_CLR; 
	DATAOUT(data); 
	LCD_WR_CLR; 
	LCD_WR_SET; 
 	LCD_CS_SET;   
}
#else//正常IO
//写寄存器函数
void LCD_WR_REG(u8 data)
{ 
	LCD_RS=0;//写地址  
 	LCD_CS=0; 
	DATAOUT(data); 
	LCD_WR=0; 
	LCD_WR=1; 
 	LCD_CS=1;   
} 	
#endif	

//写寄存器
void LCD_WriteReg(u8 LCD_Reg, u16 LCD_RegValue)
{	
	LCD_WR_REG(LCD_Reg);  
	LCD_WR_DATA(LCD_RegValue);	    		 
}	   
//读寄存器
u16 LCD_ReadReg(u8 LCD_Reg)
{										   
	u16 t;
	LCD_WR_REG(LCD_Reg);  //写入要读的寄存器号  
	GPIOB->CRL=0X88888888; //PB0-7  上拉输入
	GPIOB->CRH=0X88888888; //PB8-15 上拉输入
	GPIOB->ODR=0XFFFF;    //全部输出高
#if LCD_FAST_IO==1 //快速IO
	LCD_RS_SET;
	LCD_CS_CLR;
	//读取数据(读寄存器时,并不需要读2次)
	LCD_RD_CLR;
	delay_us(5);//FOR 8989,延时5us					   
	LCD_RD_SET;
	t=DATAIN;  
	LCD_CS_SET; 
#else
	LCD_RS=1;
	LCD_CS=0;
	//读取数据(读寄存器时,并不需要读2次)
	LCD_RD=0;					   
	LCD_RD=1;
	t=DATAIN;  
	LCD_CS=1; 
#endif  
	GPIOB->CRL=0X33333333; //PB0-7  上拉输出
	GPIOB->CRH=0X33333333; //PB8-15 上拉输出
	GPIOB->ODR=0XFFFF;    //全部输出高
	return t;  
}   
//开始写GRAM
void LCD_WriteRAM_Prepare(void)
{
	LCD_WR_REG(R34);
}	 
//LCD写GRAM
void LCD_WriteRAM(u16 RGB_Code)
{							    
	LCD_WR_DATA(RGB_Code);//写十六位GRAM
}
//从ILI93xx读出的数据为GBR格式，而我们写入的时候为RGB格式。
//通过该函数转换
//c:GBR格式的颜色值
//返回值：RGB格式的颜色值
u16 LCD_BGR2RGB(u16 c)
{
	u16  r,g,b,rgb;   
	b=(c>>0)&0x1f;
	g=(c>>5)&0x3f;
	r=(c>>11)&0x1f;	 
	rgb=(b<<11)+(g<<5)+(r<<0);		 
	return(rgb);
}		 
//读取个某点的颜色值	 
//x:0~239
//y:0~319
//返回值:此点的颜色
u16 LCD_ReadPoint(u16 x,u16 y)
{
	u16 t;	
//	if(x>=LCD_W||y>=LCD_H)return 0;//超过了范围,直接返回		   
	LCD_SetCursor(x,y);
	LCD_WR_REG(R34);       //选择GRAM地址 
	GPIOB->CRL=0X88888888; //PB0-7  上拉输入
	GPIOB->CRH=0X88888888; //PB8-15 上拉输入
	GPIOB->ODR=0XFFFF;     //全部输出高
#if LCD_FAST_IO==1 //快速IO
	LCD_RS_SET;
	LCD_CS_CLR;
	//读取数据(读GRAM时,需要读2次)
	LCD_RD_CLR;					   
	LCD_RD_SET;
	delay_us(2);//FOR 9320,延时2us					   
	//dummy READ
	LCD_RD_CLR;					   
	delay_us(2);//FOR 8989,延时2us					   
	LCD_RD_SET;
	t=DATAIN;  
	LCD_CS_SET;
#else
	LCD_RS=1;
	LCD_CS=0;
	//读取数据(读GRAM时,需要读2次)
	LCD_RD=0;					   
 	LCD_RD=1;
 	//dummy READ
	LCD_RD=0;					   
 	LCD_RD=1;
	t=DATAIN;  
	LCD_CS=1;  
#endif 
	GPIOB->CRL=0X33333333; //PB0-7  上拉输出
	GPIOB->CRH=0X33333333; //PB8-15 上拉输出
	GPIOB->ODR=0XFFFF;    //全部输出高  
	if(DeviceCode==0X4535||DeviceCode==0X4531||DeviceCode==0X8989||DeviceCode==0XB505)return t;//这几种IC直接返回颜色值
	else return LCD_BGR2RGB(t);
}
//LCD开启显示
void LCD_DisplayOn(void)
{					   
	LCD_WriteReg(R7, 0x0173); //26万色显示开启
}	 
//LCD关闭显示
void LCD_DisplayOff(void)
{	   
	LCD_WriteReg(R7, 0x0);//关闭显示 
}   
//设置光标位置
//Xpos:横坐标
//Ypos:纵坐标
__inline void LCD_SetCursor(u16 Xpos, u16 Ypos)
{
#if USE_HORIZONTAL==1 
	if(DeviceCode==0X8989)
	{
		LCD_WriteReg(0X4E, Ypos);
		LCD_WriteReg(0X4F, 319-Xpos);
    }else
	{
		LCD_WriteReg(R32, Ypos);
		LCD_WriteReg(R33, 319-Xpos);
	}							   
#else
//	if(DeviceCode==0X8989)
//	{
//		LCD_WriteReg(0X4E, Xpos);
//		LCD_WriteReg(0X4F, Ypos);
 ///   }else
///	{
		LCD_WriteReg(R32, Xpos);
		LCD_WriteReg(R33, Ypos);
//	}						    
#endif
}  
//画点
//x:0~239
//y:0~319
//POINT_COLOR:此点的颜色
void LCD_DrawPoint(u16 x,u16 y)
{
	LCD_SetCursor(x,y);//设置光标位置 
	LCD_WR_REG(R34);//开始写入GRAM
	LCD_WR_DATA(POINT_COLOR); 
} 
void LCD_DrawPoint1(u16 x,u16 y)
{	 
	fanse[jishu]=LCD_ReadPoint(x,y);
	jishu++;
	LCD_SetCursor(x,y);//设置光标位置 
	LCD_WR_REG(R34);//开始写入GRAM
	LCD_WR_DATA(POINT_COLOR); 
}
void LCD_DrawPoint2(u16 x,u16 y)
{	 
	POINT_COLOR=fanse[jishu];
	jishu++;
	LCD_SetCursor(x-deltaxx,y-deltayy);//设置光标位置 
	LCD_WR_REG(R34);//开始写入GRAM
	LCD_WR_DATA(POINT_COLOR); 
}	 
//初始化lcd
//该初始化函数可以初始化各种ILI93XX液晶,但是其他函数是基于ILI9320的!!!
//在其他型号的驱动芯片上没有测试! 
void lala(long shuzi)
{
		u32 xx;
	u8 bb[33],i=0;
	if(shuzi<0)xx=-shuzi;
	else xx=shuzi;
	if(shuzi<0){putchar('-');}
	if(xx==0)
	{
		putchar('0');goto endd;
	}
	while(xx!=0)
	{	
		bb[i]=(xx%2+48);
		i++;
		xx>>=1;
	}
	while(i!=0)
	{
		putchar(bb[--i]);	
	}
endd:	
	putchar(' ');	
}
void LCDInit(void)
{ 
 	RCC->APB2ENR|=1<<3;//先使能外设PORTB时钟
 	RCC->APB2ENR|=1<<4;//先使能外设PORTC时钟

	RCC->APB2ENR|=1<<0;    //开启辅助时钟
	JTAG_Set(SWD_ENABLE);  //开启SWD
											 
	//PORTC6~10复用推挽输出 	
	GPIOC->CRH&=0XFFFFF000;
	GPIOC->CRH|=0X00000333; 
	GPIOC->CRL&=0X00FFFFFF;
	GPIOC->CRL|=0X33000000;  
	GPIOC->ODR|=0X07C0; 	 
	//PORTB 推挽输出 	
	GPIOB->CRH=0X33333333;
	GPIOB->CRL=0X33333333; 	 
	GPIOB->ODR=0XFFFF;
	  					 
	delay_ms(50); // delay 50 ms 
	LCD_WriteReg(0x0000,0x0001);
	delay_ms(50); // delay 50 ms 
	DeviceCode = LCD_ReadReg(0x0000);   
	printf(" LCD ID:%x\n",DeviceCode); //打印LCD ID  
	if(DeviceCode==0x9325||DeviceCode==0x9328)//ILI9325
	{
  		LCD_WriteReg(0x00e7,0x0010);      
        LCD_WriteReg(0x0000,0x0001);//开启内部时钟
        LCD_WriteReg(0x0001,0x0100);     
        LCD_WriteReg(0x0002,0x0700);//电源开启                    
		//LCD_WriteReg(0x0003,(1<<3)|(1<<4) ); 	//65K  RGB
		//DRIVE TABLE(寄存器 03H)
		//BIT3=AM BIT4:5=ID0:1
		//AM ID0 ID1   FUNCATION
		// 0  0   0	   R->L D->U
		// 1  0   0	   D->U	R->L
		// 0  1   0	   L->R D->U
		// 1  1   0    D->U	L->R
		// 0  0   1	   R->L U->D
		// 1  0   1    U->D	R->L
		// 0  1   1    L->R U->D 正常就用这个.
		// 1  1   1	   U->D	L->R
        LCD_WriteReg(0x0003,(1<<12)|(3<<4)|(0<<3) );//65K    
        LCD_WriteReg(0x0004,0x0000);                                   
        LCD_WriteReg(0x0008,0x0207);	           
        LCD_WriteReg(0x0009,0x0000);         
        LCD_WriteReg(0x000a,0x0000);//display setting         
        LCD_WriteReg(0x000c,0x0001);//display setting          
        LCD_WriteReg(0x000d,0x0000);//0f3c          
        LCD_WriteReg(0x000f,0x0000);
		//电源配置
        LCD_WriteReg(0x0010,0x0000);   
        LCD_WriteReg(0x0011,0x0007);
        LCD_WriteReg(0x0012,0x0000);                                                                 
        LCD_WriteReg(0x0013,0x0000);                 
        delay_ms(50); 
        LCD_WriteReg(0x0010,0x1590);   
        LCD_WriteReg(0x0011,0x0227);
        delay_ms(50); 
        LCD_WriteReg(0x0012,0x009c);                  
        delay_ms(50); 
        LCD_WriteReg(0x0013,0x1900);   
        LCD_WriteReg(0x0029,0x0023);
        LCD_WriteReg(0x002b,0x000e);
        delay_ms(50); 
        LCD_WriteReg(0x0020,0x0000);                                                            
        LCD_WriteReg(0x0021,0x013f);           
		delay_ms(50); 
		//伽马校正
        LCD_WriteReg(0x0030,0x0007); 
        LCD_WriteReg(0x0031,0x0707);   
        LCD_WriteReg(0x0032,0x0006);
        LCD_WriteReg(0x0035,0x0704);
        LCD_WriteReg(0x0036,0x1f04); 
        LCD_WriteReg(0x0037,0x0004);
        LCD_WriteReg(0x0038,0x0000);        
        LCD_WriteReg(0x0039,0x0706);     
        LCD_WriteReg(0x003c,0x0701);
        LCD_WriteReg(0x003d,0x000f);
        delay_ms(50); 
        LCD_WriteReg(0x0050,0x0000); //水平GRAM起始位置 
        LCD_WriteReg(0x0051,0x00ef); //水平GRAM终止位置                    
        LCD_WriteReg(0x0052,0x0000); //垂直GRAM起始位置                    
        LCD_WriteReg(0x0053,0x013f); //垂直GRAM终止位置  
        
        LCD_WriteReg(0x0060,0xa700);        
        LCD_WriteReg(0x0061,0x0001); 
        LCD_WriteReg(0x006a,0x0000);
        LCD_WriteReg(0x0080,0x0000);
        LCD_WriteReg(0x0081,0x0000);
        LCD_WriteReg(0x0082,0x0000);
        LCD_WriteReg(0x0083,0x0000);
        LCD_WriteReg(0x0084,0x0000);
        LCD_WriteReg(0x0085,0x0000);
      
        LCD_WriteReg(0x0090,0x0010);     
        LCD_WriteReg(0x0092,0x0000);  
        LCD_WriteReg(0x0093,0x0003);
        LCD_WriteReg(0x0095,0x0110);
        LCD_WriteReg(0x0097,0x0000);        
        LCD_WriteReg(0x0098,0x0000);  
        //开启显示设置    
        LCD_WriteReg(0x0007,0x0133);   
        LCD_WriteReg(0x0020,0x0000);                                                            
        LCD_WriteReg(0x0021,0x013f);
	}else if(DeviceCode==0x9320||DeviceCode==0x9300||1)
	{
		LCD_WriteReg(0x00,0x0000);
		LCD_WriteReg(0x01,0x0100);	//Driver Output Control.
		LCD_WriteReg(0x02,0x0700);	//LCD Driver Waveform Control.
		LCD_WriteReg(0x03,0x1030);//Entry Mode Set.
		//LCD_WriteReg(0x03,0x1018);	//Entry Mode Set.
	
		LCD_WriteReg(0x04,0x0000);	//Scalling Control.
		LCD_WriteReg(0x08,0x0202);	//Display Control 2.(0x0207)
		LCD_WriteReg(0x09,0x0000);	//Display Control 3.(0x0000)
		LCD_WriteReg(0x0a,0x0000);	//Frame Cycle Contal.(0x0000)
		LCD_WriteReg(0x0c,(1<<0));	//Extern Display Interface Control 1.(0x0000)
		LCD_WriteReg(0x0d,0x0000);	//Frame Maker Position.
		LCD_WriteReg(0x0f,0x0000);	//Extern Display Interface Control 2.	    
		delay_ms(50); 
		LCD_WriteReg(0x07,0x0101);	//Display Control.
		delay_ms(50); 								  
		LCD_WriteReg(0x10,(1<<12)|(0<<8)|(1<<7)|(1<<6)|(0<<4));	//Power Control 1.(0x16b0)
		LCD_WriteReg(0x11,0x0007);								//Power Control 2.(0x0001)
		LCD_WriteReg(0x12,(1<<8)|(1<<4)|(0<<0));				//Power Control 3.(0x0138)
		LCD_WriteReg(0x13,0x0b00);								//Power Control 4.
		LCD_WriteReg(0x29,0x0000);								//Power Control 7.
	
		LCD_WriteReg(0x2b,(1<<14)|(1<<4));	    
		LCD_WriteReg(0x50,0);	//Set X Star
		//水平GRAM终止位置Set X End.
		LCD_WriteReg(0x51,239);	//Set Y Star
		LCD_WriteReg(0x52,0);	//Set Y End.t.
		LCD_WriteReg(0x53,319);	//
	
		LCD_WriteReg(0x60,0x2700);	//Driver Output Control.
		LCD_WriteReg(0x61,0x0001);	//Driver Output Control.
		LCD_WriteReg(0x6a,0x0000);	//Vertical Srcoll Control.
	
		LCD_WriteReg(0x80,0x0000);	//Display Position? Partial Display 1.
		LCD_WriteReg(0x81,0x0000);	//RAM Address Start? Partial Display 1.
		LCD_WriteReg(0x82,0x0000);	//RAM Address End-Partial Display 1.
		LCD_WriteReg(0x83,0x0000);	//Displsy Position? Partial Display 2.
		LCD_WriteReg(0x84,0x0000);	//RAM Address Start? Partial Display 2.
		LCD_WriteReg(0x85,0x0000);	//RAM Address End? Partial Display 2.
	
		LCD_WriteReg(0x90,(0<<7)|(16<<0));	//Frame Cycle Control.(0x0013)
		LCD_WriteReg(0x92,0x0000);	//Panel Interface Control 2.(0x0000)
		LCD_WriteReg(0x93,0x0001);	//Panel Interface Control 3.
		LCD_WriteReg(0x95,0x0110);	//Frame Cycle Control.(0x0110)
		LCD_WriteReg(0x97,(0<<8));	//
		LCD_WriteReg(0x98,0x0000);	//Frame Cycle Control.	   
		LCD_WriteReg(0x07,0x0173);	//(0x0173)
	}		 
	LCD_LED=1;//点亮背光	 
	LCD_Clear(80,109,226,106,205,248);
	{
	//	u8 i=0;
	//	for(i=0;i<60;i++)
	//	{
	//		printf("%d:,%d",i,(LCD_ReadReg(i)));
	//		putchar('\n');
	//		delay_ms(50);
	//	}
	
	}	
}  		  
  
//清屏函数
//Color:要清屏的填充色
void LCD_Real_Clear(u16 Color)
{
	u32 index=0;      
	LCD_SetCursor(0x00,0x0000);//设置光标位置 
	LCD_WriteRAM_Prepare();     //开始写入GRAM	 	  
	for(index=0;index<76800;index++)
	{
		LCD_WR_DATA(Color);    
	}
}  
void LCD_Clear(u8 a,u8 b,u8 c,u8 a1,u8 b1,u8 c1)
{
	u16 i,j;
	for(i=0;i<320;i++)
	{
		POINT_COLOR=yan(a+(a1-a)*i/319,b+(b1-b)*i/319,c+(c1-c)*i/319);
		for(j=0;j<240;j++)
		{
			LCD_DrawPoint(i,j);
		}
	}


}
//在指定区域内填充指定颜色
//区域大小:
//  (xend-xsta)*(yend-ysta)
void LCD_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color)
{          
	u16 i,j;
	u16 xlen=0;
#if USE_HORIZONTAL==1
	xlen=yend-ysta+1;	   
	for(i=xsta;i<=xend;i++)
	{
	 	LCD_SetCursor(i,ysta);      //设置光标位置 
		LCD_WriteRAM_Prepare();     //开始写入GRAM	  
		for(j=0;j<xlen;j++)LCD_WR_DATA(color);//设置光标位置 	  
	}
#else
	xlen=xend-xsta+1;	   
	for(i=ysta;i<=yend;i++)
	{
	 	LCD_SetCursor(xsta,i);      //设置光标位置 
		LCD_WriteRAM_Prepare();     //开始写入GRAM	  
		for(j=0;j<xlen;j++)LCD_WR_DATA(color);//设置光标位置 	    
	}
#endif						  	    
}  
//画线
//x1,y1:起点坐标
//x2,y2:终点坐标  
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 

	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		LCD_DrawPoint(uRow,uCol);//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}    
//画矩形
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
	LCD_DrawLine(x1,y1,x2,y1);
	LCD_DrawLine(x1,y1,x1,y2);
	LCD_DrawLine(x1,y2,x2,y2);
	LCD_DrawLine(x2,y1,x2,y2);
}
//在指定位置画一个指定大小的圆
//(x,y):中心点
//r    :半径
void LCD_DrawCircle(u16 x0,u16 y0,u8 r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //判断下个点位置的标志
	while(a<=b)
	{
		LCD_DrawPoint(x0-b,y0-a);             //3           
		LCD_DrawPoint(x0+b,y0-a);             //0           
		LCD_DrawPoint(x0-a,y0+b);             //1       
		LCD_DrawPoint(x0-b,y0-a);             //7           
		LCD_DrawPoint(x0-a,y0-b);             //2             
		LCD_DrawPoint(x0+b,y0+a);             //4               
		LCD_DrawPoint(x0+a,y0-b);             //5
		LCD_DrawPoint(x0+a,y0+b);             //6 
		LCD_DrawPoint(x0-b,y0+a);             
		a++;
		//使用Bresenham算法画圆     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 
		LCD_DrawPoint(x0+a,y0+b);
	}
} 
//在指定位置显示一个字符
//x:0~234
//y:0~308
//num:要显示的字符:" "--->"~"
//size:字体大小 12/16
//mode:叠加方式(1)还是非叠加方式(0)
//在指定位置显示一个字符
//x:0~234
//y:0~308
//num:要显示的字符:" "--->"~"
//size:字体大小 12/16
//mode:叠加方式(1)还是非叠加方式(0)
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode)
{  
#if USE_HORIZONTAL==1
#define MAX_CHAR_POSX 312
#define MAX_CHAR_POSY 232 
#else     
#define MAX_CHAR_POSX 232
#define MAX_CHAR_POSY 312
#endif 
    u8 temp;
    u8 pos,t;
    u16 x0=x;
    u16 colortemp=POINT_COLOR;      
    if(x>MAX_CHAR_POSX||y>MAX_CHAR_POSY)return;     
    //设置窗口  	   
    num=num-' ';//得到偏移后的值
    if(!mode) //非叠加方式
    {
        for(pos=0;pos<size;pos++)
    	{
            if(size==12)temp=asc2_1206[num][pos];//调用1206字体
            else temp=asc2_1608[num][pos];       //调用1608字体
            for(t=0;t<size/2;t++)
            {                 
                if(temp&0x01)POINT_COLOR=colortemp;
                else POINT_COLOR=BACK_COLOR;
                LCD_DrawPoint(x,y);	
                temp>>=1; 
        		x++;
            }
        	x=x0;
        	y++;
    	}	
    }else//叠加方式
    {
        for(pos=0;pos<size;pos++)
    	{
            if(size==12)temp=asc2_1206[num][pos];//调用1206字体
            else temp=asc2_1608[num][pos];       //调用1608字体
            for(t=0;t<size/2;t++)
            {                 
                if(temp&0x01)LCD_DrawPoint(x+t,y+pos);//画一个点     
                temp>>=1; 
            }
    	}
    }
    POINT_COLOR=colortemp;             	 	  
} 

//m^n函数
u32 mypow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}			 
//显示2个数字
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//color:颜色
//num:数值(0~4294967295);	 
void LCD_ShowNum(u16 x,u16 y,s32 num,u8 len,u8 size,u8 mode)
{         	
	u8 t,temp;
	u8 enshow=0;
	u8 fuhao=0;
	if(num<0)
	{
	fuhao=1;
	num=-num;						   
	}
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+(size/2)*t,y,' ',size,mode);
				continue;
			}else 
			{
			enshow=1;
			if(fuhao)
			LCD_ShowChar(x+(size/2)*(t-1),y,'-',size,mode); 
			} 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,mode); 
	}
} 
//显示2个数字
//x,y:起点坐标
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~99);	 
void LCD_Show2Num(u16 x,u16 y,u16 num,u8 len,u8 size,u8 mode)
{         	
	u8 t,temp;						   
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,mode); 
	}
} 
//显示字符串
//x,y:起点坐标  
//*p:字符串起始地址
//用16字体
void LCD_ShowString(u16 x,u16 y,const u8 *p)
{         
    while(*p!='\0')
    {       
        if(x>MAX_CHAR_POSX){x=0;y+=16;}
        if(y>MAX_CHAR_POSY){y=x=0;LCD_Real_Clear(WHITE);}
        LCD_ShowChar(x,y,*p,16,1);
        x+=8;
        p++;
    }  
}
void huaanniu(u16 x,u16 y,const u8 *ming,u8 neizhi)
{   u16 x1,y1,a,b,yanse=160;
	u16 xhuan;
	u8 size=0;
	xhuan=y;
	y=x;
	x=xhuan;
    a=x+1;b=y;
    POINT_COLOR=0Xdefb;
    LCD_DrawPoint(y,x);
    POINT_COLOR=0X9cd3;
    LCD_DrawPoint(y,x+1);
    POINT_COLOR=0X738E;
    for(y1=y+2;y1<y+40;y1++)
    LCD_DrawPoint(y1,x);
    POINT_COLOR=0X9cd3;
    LCD_DrawPoint(y+40,x);
    POINT_COLOR=0Xdefb;
    LCD_DrawPoint(y+41,x);
    
    
    x+=31;
    POINT_COLOR=0Xdefb;
    LCD_DrawPoint(y,x);
    POINT_COLOR=0X9cd3;
    LCD_DrawPoint(y+1,x);
    POINT_COLOR=0X738E;
    for(y1=y+2;y1<y+40;y1++)
    LCD_DrawPoint(y1,x);
    POINT_COLOR=0X9cd3;
    LCD_DrawPoint(y+40,x);
    POINT_COLOR=0Xdefb;
    LCD_DrawPoint(y+41,x);
    
    
    x-=31;
    POINT_COLOR=0X9cd3;
    LCD_DrawPoint(y,x+1);
    for(x1=x+2;x1<x+30;x1++)
    LCD_DrawPoint(y,x1);
    POINT_COLOR=0X9cd3;
    LCD_DrawPoint(y,x+30);

    y+=41;
    POINT_COLOR=0X9cd3;
    LCD_DrawPoint(y,x+1);
    for(x1=x+2;x1<x+30;x1++)
    LCD_DrawPoint(y,x1);
    POINT_COLOR=0X9cd3;
    LCD_DrawPoint(y,x+30);

    for(yanse=160;a<x+31;a++,yanse++,POINT_COLOR=yan(yanse,yanse,yanse))
    for(b=y-40;b<y;b++)
    LCD_DrawPoint(b,a);
	POINT_COLOR=BLACK;
	while(*ming!='\0')
	{
	ming+=1;
	size+=1;
	}
	if(neizhi)
	LCD_ShowString(b-20-4*size,a,ming-size);
   	else LCD_ShowString(b-20-4*size,a-20,ming-size);
   // y-=41;

   // POINT_COLOR=BLACK;
}
u16 yan(u8 a,u8 b,u8 c)
{
return(((a/8)<<11)+((b/4)<<5)+c/8);
//return(a/16*4096+(a%16/8*8+b/32)*256+(b%32/4*2+c/128)*16+c%128/8);
}
void huashubiao(u16 x,u16 y)
{	
u8 i;
	deltaxx=x-xx;
	deltayy=y-yy;
	xx=x;
	yy=y;
   	if(biaozhi==0)
	{biaozhi=1;goto miao;}
 	jishu=0;
	LCD_DrawPoint2(x+4,y);
	LCD_DrawPoint2(x-4,y);
	LCD_DrawPoint2(x,y+4);
	LCD_DrawPoint2(x,y-4);
	LCD_DrawPoint2(x+4,y-1);
	LCD_DrawPoint2(x+4,y+1);
	LCD_DrawPoint2(x+1,y-4);
	LCD_DrawPoint2(x-1,y-4);
	LCD_DrawPoint2(x-4,y+1);
	LCD_DrawPoint2(x-4,y-1);
	LCD_DrawPoint2(x+1,y+4);
	LCD_DrawPoint2(x-1,y+4);
	LCD_DrawPoint2(x+2,y+3);
	LCD_DrawPoint2(x+3,y+2);
	LCD_DrawPoint2(x-3,y-2);
	LCD_DrawPoint2(x-2,y-3);
	LCD_DrawPoint2(x-2,y+3);
	LCD_DrawPoint2(x-3,y+2);
	LCD_DrawPoint2(x+2,y-3);
	LCD_DrawPoint2(x+3,y-2);
	POINT_COLOR=yan(232,249,159);
	for(i=0;i<3;i++)
	{
	LCD_DrawPoint2(x-3,y+1-i);
	}

	for(i=0;i<5;i++)
	{
	LCD_DrawPoint2(x-2,y+2-i);
	}

	for(i=0;i<7;i++)
	{
	LCD_DrawPoint2(x-1,y+3-i);
	}

	for(i=0;i<7;i++)
	{
	LCD_DrawPoint2(x,y+3-i);
	}
	
	for(i=0;i<7;i++)
	{
	LCD_DrawPoint2(x+1,y+3-i);
	}

	for(i=0;i<5;i++)
	{
	LCD_DrawPoint2(x+2,y+2-i);
	}

	for(i=0;i<3;i++)
	{
	LCD_DrawPoint2(x+3,y+1-i);
	}
	

miao:
	jishu=0;
	 POINT_COLOR=BLACK;
	LCD_DrawPoint1(x+4,y);
	LCD_DrawPoint1(x-4,y);
	LCD_DrawPoint1(x,y+4);
	LCD_DrawPoint1(x,y-4);

 // LCD_ShowNum(56+60,186,x,3,16,0);
//  LCD_ShowNum(56+130,186,y,4,16,0);
	LCD_DrawPoint1(x+4,y-1);
//	  LCD_ShowNum(56+30,186,fanse[4],3,16,0);
	LCD_DrawPoint1(x+4,y+1);
	LCD_DrawPoint1(x+1,y-4);
	LCD_DrawPoint1(x-1,y-4);
	LCD_DrawPoint1(x-4,y+1);
	LCD_DrawPoint1(x-4,y-1);
	LCD_DrawPoint1(x+1,y+4);
	LCD_DrawPoint1(x-1,y+4);
	LCD_DrawPoint1(x+2,y+3);
	LCD_DrawPoint1(x+3,y+2);
	LCD_DrawPoint1(x-3,y-2);
	LCD_DrawPoint1(x-2,y-3);
	LCD_DrawPoint1(x-2,y+3);
	LCD_DrawPoint1(x-3,y+2);
	LCD_DrawPoint1(x+2,y-3);
	LCD_DrawPoint1(x+3,y-2);
	POINT_COLOR=yan(232,249,159);
	for(i=0;i<3;i++)
	{
	LCD_DrawPoint1(x-3,y+1-i);
	}
	POINT_COLOR=yan(206,241,171);
	for(i=0;i<5;i++)
	{
	LCD_DrawPoint1(x-2,y+2-i);
	}
	POINT_COLOR=yan(180,233,183);
	for(i=0;i<7;i++)
	{
	LCD_DrawPoint1(x-1,y+3-i);
	}
	POINT_COLOR=yan(154,225,195);
	for(i=0;i<7;i++)
	{
	LCD_DrawPoint1(x,y+3-i);
	}
	POINT_COLOR=yan(128,218,207);
	for(i=0;i<7;i++)
	{
	LCD_DrawPoint1(x+1,y+3-i);
	}
	POINT_COLOR=yan(102,210,219);
	for(i=0;i<5;i++)
	{
	LCD_DrawPoint1(x+2,y+2-i);
	}
	POINT_COLOR=yan(76,202,231);
	for(i=0;i<3;i++)
	{
	LCD_DrawPoint1(x+3,y+1-i);
	}

}



























