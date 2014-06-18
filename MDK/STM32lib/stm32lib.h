#ifndef _STM32LIB_H_
#define _STM32LIB_H_
#include <stm32f10x_lib.h>

#include "sys.h"
#include "delay.h"
//#include "LCD.h"

////////////////////////////
///核心宏////////////////

#define uint16           u16
#define uint8             u8
#define uchar u8
#define uint u16
typedef long long u64;
//////////////////////////
//////////////偶邪恶的函数们////////////////
u8 SCPE(u16 PE);
u32 max(u32 *shulie ,u32 len);
void IOConfig(GPIO_TypeDef*  ,u8 ,u32 ,u8 );
void zhongduanConfig(u8 NVIC_Channel,int i);
void waibuzhongduan(char port,u8 pin,u8 la,u8 state);
void waibuzhongduanConfig(GPIO_TypeDef*  IOBase,u8 BITx,u8 TRIM,u8 youxianji);
void usart2putchar(char c);
//void delay_s(u32 aa);
void SPIInit(void);
u8 SPIRW(u8 TxData);
void mprintf(char *shuru,...);	
u32 StringToInt(u8 * mm,u8 jinzhi);
u32 min(short *miao,u32 len);
void nop(void);
#define pc(aa)			//putchar(aa) //串口输出函数,printf用
/////////////外设地址定义，第二位是1，则RCC->APB2ENR2，以此类推//////////////////
#define abs(signed_data) (((signed_data)>0)?(signed_data):(-(signed_data)))
#define shangla			1
#define xiala 			2
#define fall 			1
#define raise			2	
#define high			1
#define low				0
#define PERTIMER2			 0X0100
#define PERTIMER3			 0x0101
#define PERTIMER4			 0x0102
#define PERTIMER5			 0x0103
#define PERTIMER6			 0x0104
#define PERTIMER7			 0x0105
#define PERTIMER12			 0x0106
#define PERTIMER13			 0x0107
#define PERTIMER14			 0x0108

#define PERWDG				 0x010B
#define PERSPI2				 0x010E
#define PERSPI3				 0x010F
#define PERUSART2			 0x0111
#define PERUSART3			 0x0112
#define PERUSART4			 0x0113
#define PERUSART5			 0x0114
#define PERI2C1				 0x0115
#define PERI2C2				 0x0116
#define PERUSB				 0x0117
#define PERCAN				 0x0119
#define PERBKP				 0X011B
#define PERPWR				 0x011C
#define PERDA				 0X011D



#define PERAFIO				 0X0200
#define PERIOA				 0x0202
#define PERIOB				 0x0203
#define PERIOC 	  		 0x0204
#define PERIOD				 0x0205
#define PERIOE				 0x0206
#define PERIOF				 0x0207
#define PERIOG				 0x0208
#define PERAD1				 0X0209
#define	PERAD2				 0X020A
#define	PERTIMER1			 0X020B
#define	PERSPI1				 0X020C
#define	PERTIMER8			 0X020D
#define	PERUSART1			 0X020E
#define	PERAD3				 0X020F
#define PERTIMER9			 0X0213
#define PERTIMER10			 0X0214
#define PERTIMER11			 0X0215



#define IOAB			 GPIOA
#define IOBB			 GPIOB
#define IOCB			 GPIOC
#define IODB			 GPIOD
#define IOEB			 GPIOE
#define IOFB			 GPIOF
#define IOGB			 GPIOG
#define PIN0			0x1
#define PIN1			0x10
#define PIN2			0x100
#define PIN3			0x1000
#define PIN4			0x10000
#define PIN5			0x100000
#define PIN6			0x1000000
#define PIN7			0x10000000
#define PIN8			0x1
#define PIN9			0x10
#define PIN10			0x100
#define PIN11			0x1000
#define PIN12			0x10000
#define PIN13			0x100000
#define PIN14			0x1000000
#define PIN15			0x10000000
#define zhongduan0		EXTI0_IRQChannel
#define zhongduan1		EXTI1_IRQChannel
#define zhongduan2		EXTI2_IRQChannel
#define zhongduan3		EXTI3_IRQChannel
#define zhongduan4		EXTI4_IRQChannel
#define zhongduan5_9	EXTI9_5_IRQChannel 
#define zhongduan10_15	EXTI15_10_IRQChannel
#define zhongduanhanshu0  EXTI0_IRQHandler
#define zhongduanhanshu1  EXTI1_IRQHandler
#define zhongduanhanshu2  EXTI2_IRQHandler
#define zhongduanhanshu3  EXTI3_IRQHandler
#define zhongduanhanshu4  EXTI4_IRQHandler
#define zhongduanhanshu5_9  EXTI9_5_IRQHandler
#define zhongduanhanshu10_15 	EXTI15_10_IRQHandler


#endif

