#include "registry.h"

#include "main.h"
#include <cstdlib>

const IRQn_Type Nvic::irqn_callback_types_[] = {SysTick_IRQn};
const Gpio::Pin::number_t Nvic::exti_callback_types_[] = {Gpio::Pin::P0};

Button Registry::BTN_USER({BTN_USER_GPIO_Port, BTN_USER_Pin}, 50);
//Led Registry::LED_ORANGE({LED_ORANGE_GPIO_Port, LED_ORANGE_Pin});
//Led Registry::LED_GREEN({LED_GREEN_GPIO_Port, LED_GREEN_Pin});
//Led Registry::LED_RED({LED_RED_GPIO_Port, LED_RED_Pin});
//Led Registry::LED_BLUE({LED_BLUE_GPIO_Port, LED_BLUE_Pin});
//FlashLed Registry::LED_ORANGEF({LED_ORANGE_GPIO_Port, LED_ORANGE_Pin}, 500);
FlashLed Registry::LED_GREENF({LED_GREEN_GPIO_Port, LED_GREEN_Pin}, 500);
//FlashLed Registry::LED_REDF({LED_RED_GPIO_Port, LED_RED_Pin}, 500);
FlashLed Registry::LED_BLUEF({LED_BLUE_GPIO_Port, LED_BLUE_Pin}, 500);
Uart Registry::UART_2(&huart2, USART2_IRQn);
WatchDog Registry::WDG_I(&hiwdg);

void f(bool *) {
  Registry::LED_BLUEF.on();
}

void uart_echo(Uart::CallbackData *data) {
  if (data->error) {
    Error_Handler();
  }
  Registry::UART_2.send(data->buffer, data->len);
  Registry::UART_2.recv();
}

int main() {
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_IWDG_Init();
  Registry::LED_GREENF.on();

  Registry::WDG_I.registerRefreshCallback();

  Nvic::FunctionCallback btn_cb(&f);
  Registry::BTN_USER.setCallback(&btn_cb);

  // UART echo.
  char c;
  Nvic::FunctionCallback uart_cb(&uart_echo);
  Registry::UART_2.recv(&c, 1, &uart_cb);

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
