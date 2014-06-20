#include <stm32f10x_lib.h>
#include "sys.h"
#include "usart.h"		
#include "delay.h"		   
#include "usb_desc.h"
#include "usb_ch341.h"

int main(void)
{		                                                               				  	
   	Stm32_Clock_Init(9);//系统时钟设置
	delay_init(72);		//延时初始化
	uart_init(72,115200);
	JTAG_Set(1);

    USB_CH341_Init();
    
	while(1)
	{

        USB_send("sdlkfj",4);
        
        USB_send("wefj",4); 
        delay_ms(500);
	}
}




