#include <stm32f10x_lib.h>	
#include "stm32lib.h"
#define bool			unsigned char
#define true 			1
#define false			0
bool SCPE(u16 PE)//(SCPE(PERIOA))
{
	switch(PE/0X100)
	{
		case 0:return false;
		case 1:RCC->APB1ENR|=1<<(PE&0xff);break;
		case 2:RCC->APB2ENR|=1<<(PE&0xff);break;
		default:return false;
	}
	return true;
}

u32 max(u32 *shulie ,u32 len)
{
	u32 max=0;
	u32 i=0;
	for(i=0;i<len;i++)
	{
		if(shulie[i]>max)
		max=shulie[i];
	}
	return max;

}

void IOConfig(GPIO_TypeDef*  IOBase,u8 ishigh,u32 PIN,u8 state)	  //(IOCB,0,PIN3|PIN2,8)//
{	if(ishigh==true)
	{
		IOBase->CRH&=0XFFFFFFFF-(0xf*PIN);
		IOBase->CRH|=state*PIN;
	} 
	else 
	{
		IOBase->CRL&=0XFFFFFFFF-(0xF*PIN);
		IOBase->CRL|=state*PIN;
	}
}
    //使能PORTA时钟

void zhongduanConfig(u8 NVIC_Channel,int i)
{
	MY_NVIC_Init(i,0,NVIC_Channel,4);			
}
void usart2putchar(char c)
{
	USART2->DR=c;				 
}
void delay_s(u32 aa)
{
	u32 i;
	for(i=0;i<aa;i++)
	delay_ms(1000);
}
//waibuzhongduanConfig(GPIOA,13,FTIR,12);
void waibuzhongduanConfig(GPIO_TypeDef*  IOBase,u8 BITx,u8 TRIM,u8 youxianji)
{
	u8 EXTADDR;
	u8 EXTOFFSET;
	EXTADDR=BITx/4;//得到中断寄存器组的编号
	EXTOFFSET=(BITx%4)*4;

	RCC->APB2ENR|=0x01;//使能io复用时钟			 

	AFIO->EXTICR[EXTADDR]&=~(0x000F<<EXTOFFSET);//清除原来设置！！！
	AFIO->EXTICR[EXTADDR]|=((((u32)IOBase)-APB2PERIPH_BASE)/0x400-2)<<EXTOFFSET;//EXTI.BITx映射到GPIOx.BITx
	
	//自动设置
	EXTI->IMR|=1<<BITx;//  开启line BITx上的中断
	//EXTI->EMR|=1<<BITx;//不屏蔽line BITx上的事件 (如果不屏蔽这句,在硬件上是可以的,但是在软件仿真的时候无法进入中断!)
 	if(TRIM&0x01)EXTI->FTSR|=1<<BITx;//line BITx上事件下降沿触发
	if(TRIM&0x02)EXTI->RTSR|=1<<BITx;//line BITx上事件上升降沿触发
	if(BITx<5)
		zhongduanConfig(BITx+6,youxianji);
	else if(BITx<10) 
		zhongduanConfig(EXTI9_5_IRQChannel,youxianji);
	else 
		zhongduanConfig(EXTI15_10_IRQChannel,youxianji);
}

/*EXTI0_IRQChannel             ((u8)0x06) 
EXTI1_IRQChannel             ((u8)0x07)  
EXTI2_IRQChannel             ((u8)0x08)  
EXTI3_IRQChannel             ((u8)0x09)  
EXTI4_IRQChannel 		
EXTI9_5_IRQChannel 
EXTI15_10_IRQChannel
EXTI0_IRQHandler
*/
//EXTI->PR=9<<0;

void SPIInit()
{	 
	SCPE(PERIOA); 
	SCPE(PERSPI1);    //SPI1时钟使能 
		   
	//这里只针对SPI口初始化
	IOConfig(IOAB,low,PIN5|PIN6|PIN7,0XB);
	PAout(5)=1;
	PAout(6)=1;
	PAout(7)=1;
	SPI1->CR1|=0<<10;//全双工模式	
	SPI1->CR1|=1<<9; //软件nss管理
	SPI1->CR1|=1<<8;  

	SPI1->CR1|=1<<2; //SPI主机
	SPI1->CR1|=0<<11;//8bit数据格式	
	//对24L01要设置 CPHA=0;CPOL=0;
	SPI1->CR1|=0<<1; //CPOL=0时空闲模式下SCK为1 
	//SPI1->CR1|=1<<1; //空闲模式下SCK为1 CPOL=1			   
	SPI1->CR1|=0<<0; //第一个时钟的下降沿,CPHA=1 CPOL=1	   
	SPI1->CR1|=7<<3; //Fsck=Fcpu/256
	SPI1->CR1|=0<<7; //MSBfirst   
	SPI1->CR1|=1<<6; //SPI设备使能
	SPIRW(0xff);//启动传输		 
}
u8 SPIRW(u8 TxData)
{		
	u8 retry=0;				 
	while((SPI1->SR&1<<1)==0)//等待发送区空	
	{
		retry++;
		if(retry>200)return 0;
	}			  
	SPI1->DR=TxData;	 	  //发送一个byte 
	retry=0;
	while((SPI1->SR&1<<0)==0) //等待接收完一个byte  
	{
		retry++;
		if(retry>200)return 0;
	}	  						    
	return SPI1->DR;          //返回收到的数据				    
}
void mprintf(char *shuru,...)
{
    int i=0;
    int canshujishu=1;
    //printf("%c",*(shuru));
   // while(*(((char *)(&shuru+4*i)))!='\0')
    while(*(char *)(shuru+i)!='\0')
    {
        char now=*(char *)(shuru+i);
        char next=*(char *)(shuru+i+1);
        switch(now)
        {

        case '%':
        {
            switch(next)
            {
                case 'd':
                {
                    char  jishu=0;
                    char xulie[20];                   
                    u32 shuchu=*((int *)((&shuru)+(canshujishu++)));
									if(shuchu==0)
									pc('0');
                    xulie[0]=xulie[0];
                    for(jishu=0;jishu<20&&shuchu!=0;jishu++)
                    {
                        xulie[jishu]=shuchu%10;
                        shuchu/=10;
                    }
                    for(;jishu>0;jishu--)
                    {
                        pc(xulie[jishu-1]+48);
                    }

                    break;
                }
                case 'b':
                {
                    char  jishu=0;
                    char xulie[50];

                    int shuchu=*((int *)((&shuru)+(canshujishu++)));
                    xulie[0]=xulie[0];
					if(shuchu==0)
					pc('0');
                    for(jishu=0;jishu<20&&shuchu!=0;jishu++)
                    {
                        xulie[jishu]=shuchu%2;
                        shuchu/=2;
                    }
                    for(;jishu>0;jishu--)
                    {
                        pc(xulie[jishu-1]+48);
                    }

                    break;
                }
                case 'H':
                {
                    char  jishu=0;
                    char xulie[20];
                    int shuchu=*((int *)((&shuru)+(canshujishu++)));
					if(shuchu==0)
					pc('0');
                    for(jishu=0;jishu<20&&shuchu!=0;jishu++)
                    {
                        xulie[jishu]=shuchu%16;
                        shuchu/=16;
                    }
                    for(;jishu>0;jishu--)
                    {
                        if(xulie[jishu-1]<10)
                         pc(xulie[jishu-1]+48);
                        else
                            pc(xulie[jishu-1]-10+'A');
                    }

                    break;
                }
                case 'h':
                {
                    char  jishu=0;
                    char xulie[20];
                    int shuchu=*((int *)((&shuru)+(canshujishu++)));
                    if(shuchu==0)
                        pc('0');
                    for(jishu=0;jishu<20&&shuchu!=0;jishu++)
                    {
                        xulie[jishu]=shuchu%16;
                        shuchu/=16;
                    }
                    for(;jishu>0;jishu--)
                    {
                        if(xulie[jishu-1]<10)
                         pc(xulie[jishu-1]+48);
                        else
                            pc(xulie[jishu-1]-10+'a');
                    }

                    break;
                }
                case 'c':
                {
                    char shuchu=*((char *)((&shuru)+(canshujishu++)));
                    pc(shuchu);
                }
            }
            i++;
            break;
        }
        default:pc(now);
        }
        i++;
    }
}
__asm void nop(void)
{
NOP

}
u32 StringToInt(u8 * mm,u8 jinzhi)
{
	u32 i=0,fangchujie=0,deshu=0;
	if(jinzhi==10)
	{
		while((mm[i]!=0x0D)&&(mm[i]!=0x0A))
		{
			 deshu*=10;
			 deshu+=(mm[i]-48);
			 i++;
			 fangchujie++;
		//	 printf("deshu:%d,i:%d",deshu,i);
			// if(fangchujie>20||mm[i]>58||mm[i<48])
			 //return 2262;
		}
		return deshu;
	}
	else return 2282;
	
}
u32 min(short *miao,u32 len)
{
	u32 i=0;
	char min=*miao;
	u32 weizhi=0;
	for(i=0;i<len;i++)
	{

			if(miao[i]<=min)
			{
				min=miao[i];
				weizhi=i;
			}
		
	//		printf("  %d:%d=%d ",i,*(miao+i),min);
	}
	return weizhi;
}
char value2pos(u32 value)
{
   u8 i=0;
   u32 value_temp=1;
   for(i=0;i<32;i++)
   {
      if(value&value_temp)
      {
         return i;
      }
      value_temp<<=1;
   }
   return 0;
}
u32 pos2value(char pos)
{
   return 1<<pos;
}

