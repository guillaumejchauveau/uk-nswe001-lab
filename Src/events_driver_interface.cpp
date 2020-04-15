#include "main.h"
#include <iostream>

void EXTI0_IRQHandler() {
  __HAL_GPIO_EXTI_CLEAR_IT(BOARD_BTN_Pin);
  std::cout << "Button" << std::endl;
}

/**
 * @brief This function handles System tick timer.
 */
void SysTick_Handler() {
  HAL_IncTick();
  //std::cout << "Tick" << std::endl;
}

/**
 * @brief This function handles Hard fault interrupt.
 */
void HardFault_Handler() {
  exit(1);
}

/**
 * @brief This function handles Memory management fault.
 */
void MemManage_Handler() {
  exit(1);
}

/**
 * @brief This function handles Pre-fetch fault, memory access fault.
 */
void BusFault_Handler() {
  exit(1);
}

/**
 * @brief This function handles Undefined instruction or illegal state.
 */
void UsageFault_Handler() {
  exit(1);
}
