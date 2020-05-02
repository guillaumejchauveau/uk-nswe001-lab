#include "interrupts.h"

const Interrupts::callback_t<void> *Interrupts::irqn_callbacks_[IRQN_TYPE_COUNT][MAX_CALLBACK_COUNT] = {};
const Interrupts::callback_t<void> *Interrupts::exti_callbacks_[EXTI_TYPE_COUNT][MAX_CALLBACK_COUNT] = {};
