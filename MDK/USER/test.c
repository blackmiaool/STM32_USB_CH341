#include <stm32f10x_lib.h>

#include "sys.h"
#include "usart.h"		
#include "delay.h"	
#include "lcd.h"	   
#include "stm32lib.h"
#include "usb_desc.h"
 u16  fanse[180];
 extern u8 buffer_in[VIRTUAL_COM_PORT_DATA_SIZE];
 extern u8 buffer_out[VIRTUAL_COM_PORT_DATA_SIZE];
 extern u32 count_in;
int main(void)
{		                                                               				  	
   	Stm32_Clock_Init(9);//系统时钟设置
	delay_init(72);		//延时初始化
	uart_init(72,115200);
	JTAG_Set(1);
    
    delay_ms(100);
    Set_USBClock();
	USB_Interrupts_Config();	
	USB_Init();	
    delay_ms(1000);
	LCDInit();
	LCD_ShowString(100,100,"start")	;
	while(1)
	{
        buffer_in[0] = 3;		
        buffer_in[1] = 4;
        buffer_in[2] = 5;
        buffer_in[3] = 6;
        buffer_in[4] = 4;
        buffer_in[5] = buffer_out[5];
        buffer_in[6] = buffer_out[6];
        buffer_in[7] = buffer_out[7];
        buffer_in[8] = buffer_out[8];
        buffer_in[9] = buffer_out[9];
        count_in=3;
        USART_To_USB_Send_Data();	
        delay_ms(1000);
	}
}




