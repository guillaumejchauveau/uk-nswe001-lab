#ifndef _REGISTRY_H_
#define _REGISTRY_H_

#include "button.h"
#include "led.h"
#include "uart.h"

namespace Registry {
extern Button USER_BTN;
extern Led ORANGE_LED;
extern Led GREEN_LED;
extern Led RED_LED;
extern Led BLUE_LED;
extern FlashLed ORANGE_FLASH_LED;
extern FlashLed GREEN_FLASH_LED;
extern FlashLed RED_FLASH_LED;
extern FlashLed BLUE_FLASH_LED;
extern Uart UART2;
}

#endif //_REGISTRY_H_
