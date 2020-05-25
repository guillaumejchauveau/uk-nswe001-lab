#ifndef _PERIPHERALS_H_
#define _PERIPHERALS_H_

#include "button.h"
#include "led.h"
#include "uart.h"
#include "wdg.h"

namespace peripheral {

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
extern SysTickTicker TICKER_SYSTICK;
extern TimTicker TICKER_TIM2;
extern TimTicker TICKER_TIM3;
extern TimTicker TICKER_TIM4;
extern TimTicker TICKER_TIM5;

} // namespace peripheral

#endif //_PERIPHERALS_H_
