#ifndef _USB_CH341_H_
#define _USB_CH341_H_
#include "stm32f10x_lib.h"
//migration part
#define USB_DISCONNECT            GPIOD  
#define USB_DISCONNECT_PIN        GPIO_Pin_0
#define RCC_APB2Periph_GPIO_DISCONNECT      RCC_APB2Periph_GPIOD
//migration end
#endif//_USB_CH341_H_

