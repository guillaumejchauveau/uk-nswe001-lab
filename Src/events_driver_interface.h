#ifndef _EVENTS_DRIVER_INTERFACE_H_
#define _EVENTS_DRIVER_INTERFACE_H_

#ifdef __cplusplus
extern "C" {
#endif

void EXTI0_IRQHandler();
void SysTick_Handler();
void HardFault_Handler();
void MemManage_Handler();
void BusFault_Handler();
void UsageFault_Handler();

#ifdef __cplusplus
}
#endif

#endif //_EVENTS_DRIVER_INTERFACE_H_
