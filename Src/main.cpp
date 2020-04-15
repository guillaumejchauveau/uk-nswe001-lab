#include "main.h"
#include "gpio.h"
#include "usart.h"
#include "led.h"

int main() {
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  HAL_NVIC_SetPriority(EXTI0_IRQn, 2,0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  Led blue(BOARD_LED_BLUE_GPIO_Port, BOARD_LED_BLUE_Pin);
  blue.on();
  while (1) {

  }
}
