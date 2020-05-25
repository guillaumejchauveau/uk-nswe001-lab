#include "peripheral/peripherals.h"

#include "main.h"
#include <cstdlib>
#include <cstdio>

using namespace peripheral;

const IRQn_Type Nvic::irqn_callback_types_[] = {SysTick_IRQn};
const Gpio::Pin::number_t Nvic::exti_callback_types_[] = {Gpio::Pin::P0};

Button peripheral::BTN_USER({BTN_USER_GPIO_Port, BTN_USER_Pin});
Led peripheral::LED_ORANGE({LED_ORANGE_GPIO_Port, LED_ORANGE_Pin});
Led peripheral::LED_GREEN({LED_GREEN_GPIO_Port, LED_GREEN_Pin});
Led peripheral::LED_RED({LED_RED_GPIO_Port, LED_RED_Pin});
Led peripheral::LED_BLUE({LED_BLUE_GPIO_Port, LED_BLUE_Pin});
//FlashLed Peripheral::LED_ORANGEF({LED_ORANGE_GPIO_Port, LED_ORANGE_Pin});
//FlashLed Peripheral::LED_GREENF({LED_GREEN_GPIO_Port, LED_GREEN_Pin});
//FlashLed Peripheral::LED_REDF({LED_RED_GPIO_Port, LED_RED_Pin});
//FlashLed Peripheral::LED_BLUEF({LED_BLUE_GPIO_Port, LED_BLUE_Pin});
Uart peripheral::UART_2(USART2, USART2_IRQn,
                        Gpio::Pin(USART2_TX_GPIO_Port, USART2_TX_Pin),
                        Gpio::Pin(USART2_RX_GPIO_Port, USART2_RX_Pin),
                        GPIO_AF7_USART2);
WatchDog peripheral::WDG_I(IWDG);
SysTickTicker peripheral::TICKER_SYSTICK;
TimTicker peripheral::TICKER_TIM2(TIM2, TIM2_IRQn);
TimTicker peripheral::TICKER_TIM3(TIM3, TIM3_IRQn);
TimTicker peripheral::TICKER_TIM4(TIM4, TIM4_IRQn);
TimTicker peripheral::TICKER_TIM5(TIM5, TIM5_IRQn);

extern void assignment1();
extern void assignment2();

int main() {
  HAL_Init();
  peripheral::UART_2.init(
    {
      .BaudRate = 921600,
      .WordLength = UART_WORDLENGTH_8B,
      .StopBits = UART_STOPBITS_1,
      .Parity = UART_PARITY_NONE,
      .Mode = UART_MODE_TX_RX,
      .HwFlowCtl = UART_HWCONTROL_NONE,
      .OverSampling = UART_OVERSAMPLING_16
    }, 1);
  printf("Reset\r\n");

  //assignment1();
  assignment2();
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler() {
  std::exit(1);
}

#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line) {
  Error_Handler();
}

#endif /* USE_FULL_ASSERT */
