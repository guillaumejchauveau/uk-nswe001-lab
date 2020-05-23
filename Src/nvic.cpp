#include "nvic.h"

const Nvic::Callback<void> *Nvic::irqn_callbacks_[IRQN_TYPE_COUNT][MAX_CALLBACK_COUNT] = {};
const Nvic::Callback<void> *Nvic::exti_callbacks_[EXTI_TYPE_COUNT][MAX_CALLBACK_COUNT] = {};
