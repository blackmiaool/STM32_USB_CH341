STM32_USB_CH341
===============
STM32 USB simulates CH341.（STM32F103.PD0 connect 1.5k then connect D+)

Why CH341?
===============
* ST's USB virtual com port demo's driver have no signature(difficult to install).
* The driver of CH341 is easy to install.
* I feel bored.            

Diagram
===============
![hello world](https://raw.githubusercontent.com/blackmiaool/STM32_USB_CH341/master/diagram.jpg)

It's not very difficult to develop. I just referred to the linux usb driver of ch341. You can simulate other chips in the same way.
