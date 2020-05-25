#ifndef _NVIC_H_
#define _NVIC_H_

#include "gpio.h"
#include "config.h"
#include "main.h"

namespace peripheral {

/**
 * Central class for interrupt handling.
 */
class Nvic {
public:
  /**
   * Abstract type for interrupt callbacks.
   * @tparam T The type of the data passed to the callback
   */
  template<typename T>
  struct Callback {
  public:
    virtual ~Callback() = default;

    /**
     * Calls the callback with a pointer to the data.
     */
    virtual void operator()(T *) const = 0;
  };

  /**
   * Callback implementation using a function pointer.
   * @tparam T The type of the data passed to the callback
   */
  template<typename T>
  struct FunctionCallback : Callback<T> {
  protected:
    void (*cb_function_)(T *);
  public:
    explicit FunctionCallback(void (*callback)(T *)) : cb_function_(callback) {
    }

    void operator()(T *data) const override {
      (*cb_function_)(data);
    }
  };

  /**
   * Callback implementation using a pointer to member function.
   * @tparam T The type of the data passed to the callback
   * @tparam M The type containing the member function
   */
  template<typename T, typename M>
  struct MemberCallback : Callback<T> {
  protected:
    void (M::*cb_member_function_)(T *);
    M *cb_member_function_context_;
  public:
    MemberCallback(void (M::*callback)(T *), M *context)
      : cb_member_function_(callback), cb_member_function_context_(context) {
    }

    void operator()(T *data) const override {
      (cb_member_function_context_->*cb_member_function_)(data);
    }
  };
protected:
  /**
   * A mapping int => IRQn_Type to find the right index for a given IRQn_Type in
   * Nvic::irqn_callbacks_.
   * Ex:
   * irqn_callback_types_[0] = SysTick_IRQn
   * irqn_callbacks_[0] = {array of SysTick_IRQn callbacks}
   */
  static const IRQn_Type irqn_callback_types_[IRQN_TYPE_COUNT];
  static const Callback<void> *irqn_callbacks_[IRQN_TYPE_COUNT][MAX_CALLBACK_COUNT];
  /**
   * Similar to Nvic::irqn_callback_types_ but for EXTI pins.
   */
  static const Gpio::Pin::number_t exti_callback_types_[EXTI_TYPE_COUNT];
  static const Callback<void> *exti_callbacks_[EXTI_TYPE_COUNT][MAX_CALLBACK_COUNT];

public:
  static void subscribe(IRQn_Type interrupt_type, const Callback<void> *callback) {
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

  static void subscribe(Gpio::Pin::number_t interrupt_type, const Callback<void> *callback) {
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

  static void subscribe(Gpio::Pin interrupt_type, const Callback<void> *callback) {
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

  static void setPriority(IRQn_Type IRQn, uint32_t PreemptPriority,
                                 uint32_t SubPriority = 0) {
    HAL_NVIC_SetPriority(IRQn, PreemptPriority, SubPriority);
  }

  static void enable(IRQn_Type IRQn) {
    HAL_NVIC_EnableIRQ(IRQn);
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

} // namespace Peripheral

#endif //_NVIC_H_
