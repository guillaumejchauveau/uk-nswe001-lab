#ifndef _BUTTON_H_
#define _BUTTON_H_
#include <utility>

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
  Button(GPIO::Pin pin, Interrupts::callback_t<bool> callback, size_t cooldown = 0)
    : pin_(pin), armed_(true), rearm_counter_(cooldown, [this](bool *rearm) {
    this->armed_ = true;
    *rearm = true;
  }), callback_(std::move(callback)) {
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

  bool isPressed() const {
    return GPIO::read(this->pin_) == GPIO_PIN_SET;
  }
};

#endif //_BUTTON_H_
