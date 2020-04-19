#include "interrupts.h"

std::unordered_map<IRQn_Type, std::list<const Interrupts::callback_t<void> *>> Interrupts::irqn_callbacks_;
std::unordered_map<GPIO::Pin::number_t, std::list<const Interrupts::callback_t<void> *>> Interrupts::exti_callbacks_;
