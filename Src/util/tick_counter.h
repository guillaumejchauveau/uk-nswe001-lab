#ifndef _TICK_COUNTER_H_
#define _TICK_COUNTER_H_

#include "peripheral/nvic.h"

namespace util {

class TickCounter {
protected:
  bool armed_ = false;
  uint32_t tick_threshold_{};
  uint32_t tick_count_ = 0;
  peripheral::Nvic::Callback<bool> *user_callback_{};
  peripheral::Nvic::MemberCallback<void, TickCounter> interrupt_callback_;

  void interruptCallback(void *) {
    if (!this->armed_) {
      return;
    }
    if (this->tick_count_ < this->tick_threshold_) {
      this->tick_count_++;
      return;
    }
    this->armed_ = false;
    this->tick_count_ = 0;
    bool rearmCounter = true;
    this->user_callback_->operator()(&rearmCounter);
    if (rearmCounter) {
      this->start();
    }
  };

public:
  TickCounter() : interrupt_callback_(&TickCounter::interruptCallback, this) {
  }

  TickCounter(peripheral::Nvic::Callback<bool> *user_callback, uint32_t tick_threshold)
    : interrupt_callback_(&TickCounter::interruptCallback, this) {
    this->init(user_callback, tick_threshold);
  }

  void init(peripheral::Nvic::Callback<bool> *user_callback, uint32_t tick_threshold) {
    assert_param(user_callback);
    assert_param(tick_threshold);
    this->user_callback_ = user_callback;
    this->tick_threshold_ = tick_threshold;
    peripheral::Nvic::subscribe(SysTick_IRQn, &this->interrupt_callback_);
  }

  ~TickCounter() {
    Error_Handler();
  }

  void start() {
    this->armed_ = true;
  }
};

} // namespace util

#endif //_TICK_COUNTER_H_
