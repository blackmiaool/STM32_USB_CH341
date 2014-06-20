#ifndef _USB_CH341_H_
#define _USB_CH341_H_
#include "stm32f10x_lib.h"
//migration part
#define USB_DISCONNECT            GPIOD  
#define USB_DISCONNECT_PIN        GPIO_Pin_0
#define RCC_APB2Periph_GPIO_DISCONNECT      RCC_APB2Periph_GPIOD
//migration end
extern u16 ch341_baud;
extern u32 ch341_state;
#define change_byte(a) do{u8 lb=a;a=(lb<<8)+(a>>8);}while(0)
#define CH341_REQ_WRITE_REG    0x9A
#define CH341_REQ_READ_REG     0x95
#define CH341_REG_BREAK1       0x05
#define CH341_REG_BREAK2       0x18
#define CH341_NBREAK_BITS_REG1 0x01
#define CH341_NBREAK_BITS_REG2 0x40

#endif//_USB_CH341_H_

