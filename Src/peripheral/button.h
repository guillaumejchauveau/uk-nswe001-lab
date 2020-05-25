#ifndef _BUTTON_H_
#define _BUTTON_H_

#include "util/counter.h"
#include "nvic.h"
#include "gpio.h"

namespace peripheral {

class Button {
protected:
  Gpio::Pin pin_;
  IRQn_Type irqn_;
  bool armed_{};
  Nvic::Callback<bool> *user_callback_{};

  Nvic::MemberCallback<void, Button> interrupt_callback_;

  void interruptCallback(void *) {
    if (!this->armed_) {
      return;
    }
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

  util::TickCounter rearm_counter_;

public:
  explicit Button(Gpio::Pin pin)
    : pin_(pin), irqn_(Nvic::getPinIRQn(pin)),
      interrupt_callback_(&Button::interruptCallback, this),
      rearm_counter_callback_(&Button::rearmCounterCallback, this) {
    Nvic::subscribe(this->pin_, &this->interrupt_callback_);
  }

  void init(Nvic::Callback<bool> *user_callback, uint32_t rearm_ticks, peripheral::Ticker *ticker,
            uint32_t PreemptPriority, uint32_t SubPriority = 0) {
    assert_param(user_callback);
    Gpio::init(this->pin_, GPIO_MODE_IT_RISING, GPIO_NOPULL);
    this->rearm_counter_.init(rearm_ticks, ticker, &this->rearm_counter_callback_);
    this->user_callback_ = user_callback;
    Nvic::setPriority(this->irqn_, PreemptPriority, SubPriority);
    Nvic::enable(this->irqn_);
    this->rearm();
  }

  ~Button() {
    Error_Handler();
  }

  void rearm() {
    this->armed_ = true;
  }

  [[nodiscard]] bool isPressed() const {
    return Gpio::read(this->pin_) == GPIO_PIN_SET;
  }
};

} // namespace peripheral

#endif //_BUTTON_H_
