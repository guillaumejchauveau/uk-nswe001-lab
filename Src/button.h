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
  Nvic::Callback<bool> *user_callback_;

  Nvic::MemberCallback<void, Button> interrupt_callback_;

  void interruptCallback(void *) {
    if (!this->armed_) {
      return;
    }
    assert_param(this->user_callback_);
    this->armed_ = false;
    bool rearm = true;
    this->user_callback_->operator()(&rearm);
    if (rearm) {
      this->rearm_counter_.start();
    }
  }

  Nvic::MemberCallback<bool, Button> rearm_counter_callback_;

  void rearmCounterCallback(bool *rearmCounter) {
    this->rearm();
    *rearmCounter = false;
  }

  TickCounter rearm_counter_;

public:
  explicit Button(Gpio::Pin pin, size_t cooldown = 0)
    : pin_(pin), irqn_(Nvic::getPinIRQn(pin)), armed_(true), user_callback_(nullptr),
      interrupt_callback_(&Button::interruptCallback, this),
      rearm_counter_callback_(&Button::rearmCounterCallback, this),
      rearm_counter_(cooldown, &this->rearm_counter_callback_) {
    Nvic::subscribe(this->pin_, &this->interrupt_callback_);
  }

  ~Button() {
    Error_Handler();
  }

  void setPriority(uint32_t PreemptPriority, uint32_t SubPriority = 0) {
    Nvic::setPriority(this->irqn_, PreemptPriority, SubPriority);
  }

  void setCallback(Nvic::Callback<bool> *callback) {
    this->user_callback_ = callback;
  }

  void rearm() {
    this->armed_ = true;
  }

  [[nodiscard]] bool isPressed() const {
    return Gpio::read(this->pin_) == GPIO_PIN_SET;
  }
};

#endif //_BUTTON_H_
