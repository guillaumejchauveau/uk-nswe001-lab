#ifndef _WATCHDOG_H_
#define _WATCHDOG_H_

#include "interrupts.h"
#include "stm32f4xx_hal_iwdg.h"

class WatchDog {
protected:
  IWDG_HandleTypeDef *handle_;
  Interrupts::callback_t<void> refreshCallback_;

public:
  explicit WatchDog(IWDG_HandleTypeDef *handle) : handle_(handle) {
    this->refreshCallback_ = [this] (void *) {
      this->refresh();
    };
  }

  void registerRefreshCallback() {
    Interrupts::subscribe(SysTick_IRQn, &this->refreshCallback_);
  }

  void refresh() {
    HAL_IWDG_Refresh(this->handle_);
  }
};

#endif //_WATCHDOG_H_
