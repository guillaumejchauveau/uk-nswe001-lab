#ifndef _IT_H_
#define _IT_H_
#include "stm32f4xx_it.h"

#ifdef __cplusplus
extern "C" {
#endif

void NMI_Handler();
void HardFault_Handler();
void MemManage_Handler();
void BusFault_Handler();
void UsageFault_Handler();
void SVC_Handler();
void DebugMon_Handler();
void PendSV_Handler();
void SysTick_Handler();
void USART2_IRQHandler();

#ifdef __cplusplus
}
#endif

#endif //_IT_H_
