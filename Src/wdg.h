#ifndef _WDG_H_
#define _WDG_H_

#include "nvic.h"
#include "stm32f4xx_hal_iwdg.h"

class WatchDog {
protected:
  IWDG_HandleTypeDef *handle_;
  Nvic::callback_t<void> refreshCallback_;

public:
  explicit WatchDog(IWDG_HandleTypeDef *handle) : handle_(handle) {
    this->refreshCallback_ = [this](void *) {
      this->refresh();
    };
  }

  ~WatchDog() {
    Error_Handler();
  }

  void registerRefreshCallback() {
    Nvic::subscribe(SysTick_IRQn, &this->refreshCallback_);
  }

  void refresh() {
    HAL_IWDG_Refresh(this->handle_);
  }
};

#endif //_WDG_H_
