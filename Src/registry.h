#ifndef _REGISTRY_H_
#define _REGISTRY_H_

#include "button.h"
#include "led.h"
#include "uart.h"
#include "wdg.h"

namespace Registry {
extern Button BTN_USER;
extern Led LED_ORANGE;
extern Led LED_GREEN;
extern Led LED_RED;
extern Led LED_BLUE;
extern FlashLed LED_ORANGEF;
extern FlashLed LED_GREENF;
extern FlashLed LED_REDF;
extern FlashLed LED_BLUEF;
extern Uart UART_2;
extern WatchDog WDG_I;
}

#endif //_REGISTRY_H_
