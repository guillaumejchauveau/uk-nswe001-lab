#ifndef _BUTTON_H_
#define _BUTTON_H_

#include "tick.h"
#include "interrupts.h"
#include "gpio.h"

class Button {
protected:
  GPIO::Pin pin_;
  bool armed_;
  TickCounter rearm_counter_;
  Interrupts::callback_t<void> interrupt_callback_;
  Interrupts::callback_t<bool> callback_;
public:
  explicit Button(GPIO::Pin pin, size_t cooldown = 0)
    : pin_(pin), armed_(true), rearm_counter_(cooldown, [this](bool *rearm) {
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
    Interrupts::subscribe(this->pin_, &this->interrupt_callback_);
  }

  ~Button() {
    Error_Handler();
  }

  void setCallback(Interrupts::callback_t<bool> callback) {
    this->callback_ = std::move(callback);
  }

  bool isPressed() const {
    return GPIO::read(this->pin_) == GPIO_PIN_SET;
  }
};

#endif //_BUTTON_H_
