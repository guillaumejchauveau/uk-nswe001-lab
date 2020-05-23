#ifndef _WDG_H_
#define _WDG_H_

#include "nvic.h"
#include "stm32f4xx_hal_iwdg.h"

class WatchDog {
protected:
  IWDG_HandleTypeDef *handle_;

  Nvic::MemberCallback<void, WatchDog> refresh_callback_;

  void refreshCallback(void *) {
    this->refresh();
  }

public:
  explicit WatchDog(IWDG_HandleTypeDef *handle)
    : handle_(handle), refresh_callback_(&WatchDog::refreshCallback, this) {
  }

  ~WatchDog() {
    Error_Handler();
  }

  void registerRefreshCallback() {
    Nvic::subscribe(SysTick_IRQn, &this->refresh_callback_);
  }

  void refresh() {
    HAL_IWDG_Refresh(this->handle_);
  }
};

#endif //_WDG_H_
