#ifndef _TICK_H_
#define _TICK_H_

#include "nvic.h"

namespace peripheral {

class Ticker {
public:
  virtual void onTick(peripheral::Nvic::Callback<void> *user_callback) = 0;
};

class SysTickTicker : public Ticker {
public:
  void onTick(peripheral::Nvic::Callback<void> *user_callback) override {
    peripheral::Nvic::subscribe(SysTick_IRQn, user_callback);
  }
};

class TimTicker : public Ticker {
protected:
  TIM_HandleTypeDef handle_{};
  IRQn_Type irqn_;
  peripheral::Nvic::Callback<void> *user_callback_{};
public:
  TimTicker(TIM_TypeDef *instance, IRQn_Type IRQn) : irqn_(IRQn) {
    this->handle_.Instance = instance;
  }

  void init(TIM_Base_InitTypeDef init, uint32_t PreemptPriority, uint32_t SubPriority = 0) {
    this->handle_.Init = init;
    if (HAL_TIM_Base_Init(&this->handle_) != HAL_OK) {
      Error_Handler();
    }
    Nvic::setPriority(this->irqn_, PreemptPriority, SubPriority);
    Nvic::enable(this->irqn_);
    HAL_TIM_Base_Start_IT(&this->handle_);
  }

  TIM_HandleTypeDef *getHandle() {
    return &this->handle_;
  }

  void onTick(peripheral::Nvic::Callback<void> *user_callback) override {
    assert_param(user_callback);
    this->user_callback_ = user_callback;
  }

  void _handlePeriodElapsed() {
    if (this->user_callback_) {
      this->user_callback_->operator()(nullptr);
    }
  }
};

} // namespace peripheral

#endif //_TICK_H_
