#include "nvic.h"
#include "peripherals.h"
#include "it.h"

#include "main.h"
#include <stm32f4xx_hal_uart.h>
#include <stm32f4xx_hal_tim.h>

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
/*void SVC_Handler() {
  //Nvic::dispatch(SVCall_IRQn);
}*/

/**
 * @brief This function handles Debug monitor.
 */
void DebugMon_Handler() {
  //Nvic::dispatch(DebugMonitor_IRQn);
}

/**
 * @brief This function handles Pendable request for system service.
 */
/*void PendSV_Handler() {
  //Nvic::dispatch(PendSV_IRQn);
}*/

/**
 * @brief This function handles System tick timer.
 */
/*void SysTick_Handler() {
  HAL_IncTick();
  Nvic::dispatch(SysTick_IRQn);
}*/

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
  if (htim->Instance == TIM1) {
    HAL_IncTick();
  }
}

/**
  * @brief This function handles USART2 global interrupt.
  */
void USART2_IRQHandler() {
  HAL_UART_IRQHandler(peripheral::UART_2.getHandle());
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
