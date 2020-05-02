#ifndef _INTERRUPTS_H_
#define _INTERRUPTS_H_

#include "gpio.h"
#include "config.h"
#include "main.h"
#include <functional>

class Interrupts {
public:
  template<typename T>
  using callback_t = std::function<void(T *)>;
protected:
  static const IRQn_Type irqn_callback_types_[IRQN_TYPE_COUNT];
  static const callback_t<void> *irqn_callbacks_[IRQN_TYPE_COUNT][MAX_CALLBACK_COUNT];
  static const GPIO::Pin::number_t exti_callback_types_[EXTI_TYPE_COUNT];
  static const callback_t<void> *exti_callbacks_[EXTI_TYPE_COUNT][MAX_CALLBACK_COUNT];

public:
  static void subscribe(IRQn_Type interrupt_type, const callback_t<void> *callback) {
    size_t i = 0;
    for (auto stored_type : Interrupts::irqn_callback_types_) {
      if (stored_type == interrupt_type) {
        size_t j;
        for (j = 0; Interrupts::irqn_callbacks_[i][j] != nullptr; j++) {
        }
        Interrupts::irqn_callbacks_[i][j] = callback;
        break;
      }
      i++;
    }
  }

  static void subscribe(GPIO::Pin::number_t interrupt_type, const callback_t<void> *callback) {
    size_t i = 0;
    for (auto stored_type : Interrupts::exti_callback_types_) {
      if (stored_type == interrupt_type) {
        size_t j;
        for (j = 0; Interrupts::exti_callbacks_[i][j] != nullptr; j++) {
        }
        Interrupts::exti_callbacks_[i][j] = callback;
        break;
      }
      i++;
    }
  }

  static void subscribe(GPIO::Pin interrupt_type, const callback_t<void> *callback) {
    Interrupts::subscribe(interrupt_type.number, callback);
  }

  static void dispatch(IRQn_Type interrupt_type) {
    size_t i = 0;
    for (auto stored_type : Interrupts::irqn_callback_types_) {
      if (stored_type == interrupt_type) {
        for (auto callback : Interrupts::irqn_callbacks_[i]) {
          if (callback == nullptr) {
            break;
          }
          callback->operator()(nullptr);
        }
        break;
      }
      i++;
    }
  }

  static void dispatch(GPIO::Pin::number_t interrupt_type) {
    size_t i = 0;
    for (auto stored_type : Interrupts::exti_callback_types_) {
      if (stored_type == interrupt_type) {
        for (auto callback : Interrupts::exti_callbacks_[i]) {
          if (callback == nullptr) {
            break;
          }
          callback->operator()(nullptr);
        }
        break;
      }
      i++;
    }
  }

  Interrupts() = delete;
  ~Interrupts() = delete;
};

#endif //_INTERRUPTS_H_
