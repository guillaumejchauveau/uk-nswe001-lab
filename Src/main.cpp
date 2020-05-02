#include "registry.h"

#include "main.h"
#include <cstdlib>

const IRQn_Type Interrupts::irqn_callback_types_[] = {SysTick_IRQn};
const GPIO::Pin::number_t Interrupts::exti_callback_types_[] = {GPIO::Pin::P0};

Button Registry::USER_BTN({USER_BTN_GPIO_Port, USER_BTN_Pin}, 50);
//Led Registry::ORANGE_LED({ORANGE_LED_GPIO_Port, ORANGE_LED_Pin});
//Led Registry::GREEN_LED({GREEN_LED_GPIO_Port, GREEN_LED_Pin});
//Led Registry::RED_LED({RED_LED_GPIO_Port, RED_LED_Pin});
//Led Registry::BLUE_LED({BLUE_LED_GPIO_Port, BLUE_LED_Pin});
//FlashLed Registry::ORANGE_FLASH_LED({ORANGE_LED_GPIO_Port, ORANGE_LED_Pin}, 500);
FlashLed Registry::GREEN_FLASH_LED({GREEN_LED_GPIO_Port, GREEN_LED_Pin}, 500);
//FlashLed Registry::RED_FLASH_LED({RED_LED_GPIO_Port, RED_LED_Pin}, 500);
FlashLed Registry::BLUE_FLASH_LED({BLUE_LED_GPIO_Port, BLUE_LED_Pin}, 500);
Uart Registry::UART2(&huart2);
WatchDog Registry::IWDG_(&hiwdg);

int main() {
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_IWDG_Init();
  Registry::GREEN_FLASH_LED.on();

  Registry::IWDG_.registerRefreshCallback();

  Registry::USER_BTN.setCallback([](void *) {
    Registry::BLUE_FLASH_LED.on();
  });

  char c;
  Registry::UART2.recv(&c, 1, [](Uart::CallbackData *data) {
    if (data->error) {
      Error_Handler();
    }
    Registry::UART2.send(data->buffer, data->len);
    Registry::UART2.recv();
  });

  while (true) {

  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler() {
  std::exit(1);
}
