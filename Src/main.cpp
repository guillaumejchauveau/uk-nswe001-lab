#include "led.h"
#include "button.h"

#include "main.h"

int main() {
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_USART2_UART_Init();

  FlashLed blue({BOARD_LED_BLUE_GPIO_Port, BOARD_LED_BLUE_Pin}, 500);
  Button user({BOARD_BTN_GPIO_Port, BOARD_BTN_Pin}, [&blue] (void *) {
    blue.on();
  }, 50);

  while (true) {

  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler() {
  exit(1);
}
