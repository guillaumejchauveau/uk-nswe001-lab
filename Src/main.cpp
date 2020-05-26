#include "peripheral/peripherals.h"

#include "main.h"
#include "cmsis_os.h"
#include <cstdlib>
#include <cstdio>

using namespace peripheral;

const IRQn_Type Nvic::irqn_callback_types_[] = {};
const Gpio::Pin::number_t Nvic::exti_callback_types_[] = {};

Led peripheral::LED_ORANGE({LED_ORANGE_GPIO_Port, LED_ORANGE_Pin});
Led peripheral::LED_GREEN({LED_GREEN_GPIO_Port, LED_GREEN_Pin});
Led peripheral::LED_RED({LED_RED_GPIO_Port, LED_RED_Pin});
Led peripheral::LED_BLUE({LED_BLUE_GPIO_Port, LED_BLUE_Pin});
Uart peripheral::UART_2(USART2, USART2_IRQn,
                        Gpio::Pin(USART2_TX_GPIO_Port, USART2_TX_Pin),
                        Gpio::Pin(USART2_RX_GPIO_Port, USART2_RX_Pin),
                        GPIO_AF7_USART2);

[[noreturn]]
void task1(void *) {
  while (true) {
    peripheral::LED_GREEN.toggle();
    printf("green [\r\n");
    printf("green led %d\r\n", peripheral::LED_GREEN.isOn());
    printf("] green\r\n");
    osDelay(100);
  }
}

[[noreturn]]
void task2(void *) {
  while (true) {
    peripheral::LED_ORANGE.toggle();
    printf("orange [\r\n");
    printf("orange led %d\r\n", peripheral::LED_ORANGE.isOn());
    printf("] orange\r\n");
    osDelay(150);
  }
}

[[noreturn]]
void task3(void *) {
  while (true) {
    peripheral::LED_RED.toggle();
    printf("red [\r\n");
    printf("red led %d\r\n", peripheral::LED_RED.isOn());
    printf("] red\r\n");
    osDelay(250);
  }
}

[[noreturn]]
void task4(void *) {
  while (true) {
    peripheral::LED_BLUE.toggle();
    printf("blue [\r\n");
    printf("blue led %d\r\n", peripheral::LED_BLUE.isOn());
    printf("] blue\r\n");
    osDelay(400);
  }
}

int main() {
  HAL_Init();
  peripheral::UART_2.init(
    {
      .BaudRate = 9600,
      .WordLength = UART_WORDLENGTH_8B,
      .StopBits = UART_STOPBITS_1,
      .Parity = UART_PARITY_NONE,
      .Mode = UART_MODE_TX_RX,
      .HwFlowCtl = UART_HWCONTROL_NONE,
      .OverSampling = UART_OVERSAMPLING_16
    }, 5);
  printf("Reset\r\n");
  peripheral::LED_BLUE.init();
  peripheral::LED_GREEN.init();
  peripheral::LED_ORANGE.init();
  peripheral::LED_RED.init();

  osKernelInitialize();
  osThreadAttr_t task_attr{};
  task_attr.name = "task1";
  task_attr.priority = (osPriority_t) osPriorityNormal5;
  task_attr.stack_size = 128 * 8;
  auto task1Handle = osThreadNew(task1, nullptr, &task_attr);
  task_attr.name = "task2";
  task_attr.priority = (osPriority_t) osPriorityNormal4;
  auto task2Handle = osThreadNew(task2, nullptr, &task_attr);
  task_attr.name = "task3";
  task_attr.priority = (osPriority_t) osPriorityNormal3;
  auto task3Handle = osThreadNew(task3, nullptr, &task_attr);
  task_attr.name = "task4";
  task_attr.priority = (osPriority_t) osPriorityNormal2;
  auto task4Handle = osThreadNew(task4, nullptr, &task_attr);
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  Error_Handler();
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
