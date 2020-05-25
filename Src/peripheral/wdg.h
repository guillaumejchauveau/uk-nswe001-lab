#ifndef _WDG_H_
#define _WDG_H_

#include "nvic.h"
#include <stm32f4xx_hal_iwdg.h>

namespace peripheral {

class WatchDog {
protected:
  IWDG_HandleTypeDef handle_{};

  Nvic::MemberCallback<void, WatchDog> refresh_callback_;

  void refreshCallback(void *) {
    this->refresh();
  }

public:
  explicit WatchDog(IWDG_TypeDef *instance)
    : refresh_callback_(&WatchDog::refreshCallback, this) {
    this->handle_.Instance = instance;
  }

  void init(IWDG_InitTypeDef init, Ticker *ticker) {
    this->handle_.Init = init;
    if (HAL_IWDG_Init(&this->handle_) != HAL_OK) {
      Error_Handler();
    }
    ticker->onTick(&this->refresh_callback_);
  }

  IWDG_HandleTypeDef *getHandle() {
    return &this->handle_;
  }

  ~WatchDog() {
    Error_Handler();
  }

  void refresh() {
    HAL_IWDG_Refresh(&this->handle_);
  }
};

} // namespace peripheral

#endif //_WDG_H_
