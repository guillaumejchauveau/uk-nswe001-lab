#ifndef _NVIC_H_
#define _NVIC_H_

#include "gpio.h"
#include "config.h"
#include "main.h"
#include <functional>

class Nvic {
public:
  template<typename T>
  using callback_t = std::function<void(T *)>;
protected:
  static const IRQn_Type irqn_callback_types_[IRQN_TYPE_COUNT];
  static const callback_t<void> *irqn_callbacks_[IRQN_TYPE_COUNT][MAX_CALLBACK_COUNT];
  static const Gpio::Pin::number_t exti_callback_types_[EXTI_TYPE_COUNT];
  static const callback_t<void> *exti_callbacks_[EXTI_TYPE_COUNT][MAX_CALLBACK_COUNT];

public:
  static void subscribe(IRQn_Type interrupt_type, const callback_t<void> *callback) {
    size_t i = 0;
    for (auto stored_type : Nvic::irqn_callback_types_) {
      if (stored_type == interrupt_type) {
        size_t j;
        for (j = 0; Nvic::irqn_callbacks_[i][j] != nullptr; j++) {
        }
        Nvic::irqn_callbacks_[i][j] = callback;
        break;
      }
      i++;
    }
  }

  static void subscribe(Gpio::Pin::number_t interrupt_type, const callback_t<void> *callback) {
    size_t i = 0;
    for (auto stored_type : Nvic::exti_callback_types_) {
      if (stored_type == interrupt_type) {
        size_t j;
        for (j = 0; Nvic::exti_callbacks_[i][j] != nullptr; j++) {
        }
        Nvic::exti_callbacks_[i][j] = callback;
        break;
      }
      i++;
    }
  }

  static void subscribe(Gpio::Pin interrupt_type, const callback_t<void> *callback) {
    Nvic::subscribe(interrupt_type.number, callback);
  }

  static void dispatch(IRQn_Type interrupt_type) {
    size_t i = 0;
    for (auto stored_type : Nvic::irqn_callback_types_) {
      if (stored_type == interrupt_type) {
        for (auto callback : Nvic::irqn_callbacks_[i]) {
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

  static void dispatch(Gpio::Pin::number_t interrupt_type) {
    size_t i = 0;
    for (auto stored_type : Nvic::exti_callback_types_) {
      if (stored_type == interrupt_type) {
        for (auto callback : Nvic::exti_callbacks_[i]) {
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

  static void setPriority(IRQn_Type IRQn, uint32_t PreemptPriority, uint32_t SubPriority = 0) {
    HAL_NVIC_SetPriority(IRQn, PreemptPriority, SubPriority);
  }

  static IRQn_Type getPinIRQn(Gpio::Pin pin) {
    switch (pin.number) {
      case Gpio::Pin::P0:
        return EXTI0_IRQn;
      case Gpio::Pin::P1:
        return EXTI1_IRQn;
      case Gpio::Pin::P2:
        return EXTI2_IRQn;
      case Gpio::Pin::P3:
        return EXTI3_IRQn;
      case Gpio::Pin::P4:
        return EXTI4_IRQn;
      case Gpio::Pin::P5:
      case Gpio::Pin::P6:
      case Gpio::Pin::P7:
      case Gpio::Pin::P8:
      case Gpio::Pin::P9:
        return EXTI9_5_IRQn;
      case Gpio::Pin::P10:
      case Gpio::Pin::P11:
      case Gpio::Pin::P12:
      case Gpio::Pin::P13:
      case Gpio::Pin::P14:
      case Gpio::Pin::P15:
        return EXTI15_10_IRQn;
      case Gpio::Pin::P_All:
        break;
    }
    Error_Handler();
  }

  Nvic() = delete;
  ~Nvic() = delete;
};

#endif //_NVIC_H_
