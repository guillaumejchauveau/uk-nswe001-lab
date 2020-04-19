#include "interrupts.h"
#include "interrupts_handlers.h"

#include "main.h"
#include <stm32f4xx_hal_gpio.h>
#include <stm32f4xx_hal_uart.h>
#include <iostream>

/**
 * @brief This function handles Non maskable interrupt.
 */
void NMI_Handler() {
  Interrupts::dispatch(NonMaskableInt_IRQn);
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
  Interrupts::dispatch(MemoryManagement_IRQn);
  exit(1);
}

/**
 * @brief This function handles Pre-fetch fault, memory access fault.
 */
void BusFault_Handler() {
  Interrupts::dispatch(BusFault_IRQn);
  exit(1);
}

/**
 * @brief This function handles Undefined instruction or illegal state.
 */
void UsageFault_Handler() {
  Interrupts::dispatch(UsageFault_IRQn);
  exit(1);
}

/**
 * @brief This function handles System service call via SWI instruction.
 */
void SVC_Handler() {
  Interrupts::dispatch(SVCall_IRQn);
}

/**
 * @brief This function handles Debug monitor.
 */
void DebugMon_Handler() {
  Interrupts::dispatch(DebugMonitor_IRQn);
}

/**
 * @brief This function handles Pendable request for system service.
 */
void PendSV_Handler() {
  Interrupts::dispatch(PendSV_IRQn);
}

/**
 * @brief This function handles System tick timer.
 */
void SysTick_Handler() {
  HAL_IncTick();
  Interrupts::dispatch(SysTick_IRQn);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
  Interrupts::dispatch(GPIO::Pin::convert_number(GPIO_Pin));
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {

}

/**
  * @brief This function handles USART2 global interrupt.
  */
/*void USART2_IRQHandler() {
  Interrupts::dispatch(USART2_IRQn);
}*/
