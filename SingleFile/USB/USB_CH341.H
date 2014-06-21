//write by blackmiaool  email:blackmiaool@gmail.com 2014.6.20
#ifndef _USB_CH341_H_
#define _USB_CH341_H_

//migration part
#define USB_DISCONNECT            GPIOD  
#define USB_DISCONNECT_PIN        GPIO_Pin_0
#define RCC_APB2Periph_GPIO_DISCONNECT      RCC_APB2Periph_GPIOD
//migration end


extern u16 ch341_baud;
void USB_CH341_Init(void);
void USB_Cable_Config (FunctionalState NewState);
void USB_To_USART_Send_Data(u8* data_buffer, u8 Nb_bytes);
void USART_To_USB_Send_Data(void);
void USB_send(u8 *buf,u8 lenth);
void USB_receive(const u8 *buf,u8 lenth);




#endif//_USB_CH341_H_

