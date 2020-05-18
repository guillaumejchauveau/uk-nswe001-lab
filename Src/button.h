#ifndef _BUTTON_H_
#define _BUTTON_H_

#include "tick.h"
#include "nvic.h"
#include "gpio.h"

class Button {
protected:
  Gpio::Pin pin_;
  IRQn_Type irqn_;
  bool armed_;
  TickCounter rearm_counter_;
  Nvic::callback_t<void> interrupt_callback_;
  Nvic::callback_t<bool> callback_;
public:
  explicit Button(Gpio::Pin pin, size_t cooldown = 0)
    : pin_(pin), irqn_(Nvic::getPinIRQn(pin)), armed_(true), rearm_counter_(cooldown,
                                                                            [this](bool *rearm) {
                                                                              this->armed_ = true;
                                                                              *rearm = false;
                                                                            }) {
    this->interrupt_callback_ = [this](void *) {
      if (!this->armed_) {
        return;
      }
      this->armed_ = false;
      assert_param(this->callback_);
      bool rearm = true;
      this->callback_.operator()(&rearm);
      if (rearm) {
        this->rearm_counter_.start();
      }
    };
    Nvic::subscribe(this->pin_, &this->interrupt_callback_);
  }

  ~Button() {
    Error_Handler();
  }

  void setPriority(uint32_t PreemptPriority, uint32_t SubPriority = 0) {
    Nvic::setPriority(this->irqn_, PreemptPriority, SubPriority);
  }

  void setCallback(Nvic::callback_t<bool> callback) {
    this->callback_ = std::move(callback);
  }

  [[nodiscard]] bool isPressed() const {
    return Gpio::read(this->pin_) == GPIO_PIN_SET;
  }
};

#endif //_BUTTON_H_
