#include "stm32f10x_it.h"
#include "usb_lib.h"
#include "usb_prop.h"
#include "usb_desc.h"
#include "hw_config.h"
#include "platform_config.h"
#include "usb_pwr.h"
#include "usb_ch341.h"
#include "usb_init.h"
#include "delay.h"
USART_InitTypeDef USART_InitStructure;
u16 ch341_baud;

u8 buffer_in[VIRTUAL_COM_PORT_DATA_SIZE];
extern  u32 count_in;
void USB_CH341_Init()
{
    Set_USBClock();
	USB_Interrupts_Config();	
	USB_Init();	
    delay_ms(1000);
    delay_ms(1000);
}

void USB_Cable_Config(FunctionalState NewState)
{
  if (NewState != DISABLE)
  {
    GPIO_SetBits(USB_DISCONNECT, USB_DISCONNECT_PIN);
  }
  else
  {
    GPIO_ResetBits(USB_DISCONNECT, USB_DISCONNECT_PIN);
  }
}

void USB_send(u8 *buf,u8 lenth)
{
  UserToPMABufferCopy(buf, ENDP2_TXADDR, lenth);
  SetEPTxCount(ENDP2, lenth);
  SetEPTxValid(ENDP2);
  while(GetEPTxStatus(2)!=32);
}
void USB_receive(const u8 *buf,u8 lenth)
{
  //add your own code and remove this 
  USB_send((u8 *)buf,lenth);
}


