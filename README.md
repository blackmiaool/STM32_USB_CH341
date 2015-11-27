STM32_USB_CH341
===============
STM32 USB simulate CH341.（STM32F103.PD0 connect 1.5k then connect D+)

Why simulate CH341?
===============
* ST's USB virtual com port demo's driver have no signature(difficult to install).
* I feel bored.            

Diagram
===============
![hello world](https://raw.githubusercontent.com/blackmiaool/STM32_USB_CH341/master/diagram.jpg)

It's not very diffcult to develop. I just refer to linux usb-host open source driver of ch341.
所以你可以去linux里面翻出来这个驱动自己实现一下。
