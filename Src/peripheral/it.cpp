#include "nvic.h"
#include "peripherals.h"
#include "it.h"

#include "main.h"
#include <stm32f4xx_hal_gpio.h>
#include <stm32f4xx_hal_uart.h>
#include <stm32f4xx_hal_tim.h>
#include <cstdio>

using namespace peripheral;

/**
 * @brief This function handles Non maskable interrupt.
 */
void NMI_Handler() {
  //Nvic::dispatch(NonMaskableInt_IRQn);
}

/**
 * @brief This function handles Hard fault interrupt.
 */
void HardFault_Handler() {
  Error_Handler();
}

/**
 * @brief This function handles Memory management fault.
 */
void MemManage_Handler() {
  //Nvic::dispatch(MemoryManagement_IRQn);
  Error_Handler();
}

/**
 * @brief This function handles Pre-fetch fault, memory access fault.
 */
void BusFault_Handler() {
  //Nvic::dispatch(BusFault_IRQn);
  Error_Handler();
}

/**
 * @brief This function handles Undefined instruction or illegal state.
 */
void UsageFault_Handler() {
  //Nvic::dispatch(UsageFault_IRQn);
  Error_Handler();
}

/**
 * @brief This function handles System service call via SWI instruction.
 */
void SVC_Handler() {
  //Nvic::dispatch(SVCall_IRQn);
}

/**
 * @brief This function handles Debug monitor.
 */
void DebugMon_Handler() {
  //Nvic::dispatch(DebugMonitor_IRQn);
}

/**
 * @brief This function handles Pendable request for system service.
 */
void PendSV_Handler() {
  //Nvic::dispatch(PendSV_IRQn);
}

/**
 * @brief This function handles System tick timer.
 */
void SysTick_Handler() {
  HAL_IncTick();
  Nvic::dispatch(SysTick_IRQn);
}

/**
  * @brief This function handles EXTI line0 interrupt.
  */
void EXTI0_IRQHandler() {
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}

/**
  * @brief This function handles TIM2 global interrupt.
  */
void TIM2_IRQHandler() {
  HAL_TIM_IRQHandler(peripheral::TICKER_TIM2.getHandle());
}

/**
  * @brief This function handles TIM3 global interrupt.
  */
void TIM3_IRQHandler() {
  HAL_TIM_IRQHandler(peripheral::TICKER_TIM3.getHandle());
}

/**
  * @brief This function handles TIM4 global interrupt.
  */
void TIM4_IRQHandler() {
  HAL_TIM_IRQHandler(peripheral::TICKER_TIM4.getHandle());
}

/**
  * @brief This function handles TIM5 global interrupt.
  */
void TIM5_IRQHandler() {
  HAL_TIM_IRQHandler(peripheral::TICKER_TIM5.getHandle());
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
  if (htim == peripheral::TICKER_TIM2.getHandle()) {
    peripheral::TICKER_TIM2._handlePeriodElapsed();
  } else if (htim == peripheral::TICKER_TIM3.getHandle()) {
    peripheral::TICKER_TIM3._handlePeriodElapsed();
  } else if (htim == peripheral::TICKER_TIM4.getHandle()) {
    peripheral::TICKER_TIM4._handlePeriodElapsed();
  } else if (htim == peripheral::TICKER_TIM5.getHandle()) {
    peripheral::TICKER_TIM5._handlePeriodElapsed();
  }
}

/**
  * @brief This function handles USART2 global interrupt.
  */
void USART2_IRQHandler() {
  HAL_UART_IRQHandler(peripheral::UART_2.getHandle());
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
  Nvic::dispatch(Gpio::Pin::convert_number(GPIO_Pin));
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
  if (huart == peripheral::UART_2.getHandle()) {
    peripheral::UART_2._handleTxCplt();
  }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
  if (huart == peripheral::UART_2.getHandle()) {
    peripheral::UART_2._handleRxCplt();
  }
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart) {
  if (huart == peripheral::UART_2.getHandle()) {
    peripheral::UART_2._handleError();
  }
}
