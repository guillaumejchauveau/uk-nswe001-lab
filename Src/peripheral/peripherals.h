#ifndef _PERIPHERALS_H_
#define _PERIPHERALS_H_

#include "led.h"
#include "uart.h"

namespace peripheral {

extern Led LED_ORANGE;
extern Led LED_GREEN;
extern Led LED_RED;
extern Led LED_BLUE;
extern Uart UART_2;

} // namespace peripheral

#endif //_PERIPHERALS_H_
