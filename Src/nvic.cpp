#include "nvic.h"

const Nvic::callback_t<void> *Nvic::irqn_callbacks_[IRQN_TYPE_COUNT][MAX_CALLBACK_COUNT] = {};
const Nvic::callback_t<void> *Nvic::exti_callbacks_[EXTI_TYPE_COUNT][MAX_CALLBACK_COUNT] = {};
