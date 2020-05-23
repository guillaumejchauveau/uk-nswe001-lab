#ifndef _TICK_H_
#define _TICK_H_

#include "nvic.h"

class TickCounter {
protected:
  bool armed_ = false;
  size_t tick_threshold_;
  size_t tick_count_ = 0;
  Nvic::Callback<bool> *user_callback_;
  Nvic::MemberCallback<void, TickCounter> interrupt_callback_;

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
    assert_param(this->user_callback_);
    bool rearmCounter = true;
    this->user_callback_->operator()(&rearmCounter);
    if (rearmCounter) {
      this->start();
    }
  };

public:
  TickCounter(size_t tick_threshold, Nvic::Callback<bool> *user_callback)
    : tick_threshold_(tick_threshold), user_callback_(user_callback),
      interrupt_callback_(&TickCounter::interruptCallback, this) {
    Nvic::subscribe(SysTick_IRQn, &this->interrupt_callback_);
  }

  ~TickCounter() {
    Error_Handler();
  }

  void start() {
    this->armed_ = true;
  }

  void setThreshold(size_t tick_threshold) {
    this->tick_threshold_ = tick_threshold;
  }
};

#endif //_TICK_H_
