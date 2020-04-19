#ifndef _TICK_H_
#define _TICK_H_
#include <utility>

#include "interrupts.h"

class TickCounter {
protected:
  bool armed_ = false;
  size_t tick_threshold_;
  size_t tick_count_ = 0;
  Interrupts::callback_t<void> interrupt_callback_;
  Interrupts::callback_t<bool> callback_;

public:
  TickCounter(size_t tick_threshold, Interrupts::callback_t<bool> callback)
    : tick_threshold_(tick_threshold), callback_(std::move(callback)) {
    this->interrupt_callback_ = [this] (void *) {
      if (!this->armed_) {
        return;
      }
      if (this->tick_count_ < this->tick_threshold_) {
        this->tick_count_++;
        return;
      }
      this->armed_ = false;
      this->tick_count_ = 0;
      assert_param(this->callback_);
      bool rearm = true;
      this->callback_.operator()(&rearm);
      if (rearm) {
        this->start();
      }
    };
    Interrupts::subscribe(SysTick_IRQn, &this->interrupt_callback_);
  }

  ~TickCounter() {
    Error_Handler();
  }

  void start() {
    this->armed_ = true;
  }
};

#endif //_TICK_H_
