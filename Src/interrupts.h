#ifndef _INTERRUPTS_H_
#define _INTERRUPTS_H_

#include "gpio.h"
#include "main.h"
#include <list>
#include <unordered_map>
#include <functional>

class Interrupts {
public:
  template<typename T>
  using callback_t = std::function<void(T *)>;
protected:
  static std::unordered_map<IRQn_Type, std::list<const callback_t<void> *>> irqn_callbacks_;
  static std::unordered_map<GPIO::Pin::number_t, std::list<const callback_t<void> *>> exti_callbacks_;

  static void dispatch(std::list<const callback_t<void> *> &handlers) {
    for (auto &handler : handlers) {
      assert_param(handler && *handler);
      handler->operator()(nullptr);
    }
  }

public:
  static void subscribe(IRQn_Type interrupt_type, const callback_t<void> *callback) {
    Interrupts::irqn_callbacks_[interrupt_type].push_back(callback);
  }

  static void subscribe(GPIO::Pin::number_t interrupt_type, const callback_t<void> *callback) {
    Interrupts::exti_callbacks_[interrupt_type].push_back(callback);
  }

  static void subscribe(GPIO::Pin interrupt_type, const callback_t<void> *callback) {
    Interrupts::subscribe(interrupt_type.number, callback);
  }

  static void dispatch(IRQn_Type interrupt_type) {
    Interrupts::dispatch(Interrupts::irqn_callbacks_[interrupt_type]);
  }

  static void dispatch(GPIO::Pin::number_t interrupt_type) {
    Interrupts::dispatch(Interrupts::exti_callbacks_[interrupt_type]);
  }

  Interrupts() = delete;
  ~Interrupts() = delete;
};

#endif //_INTERRUPTS_H_
