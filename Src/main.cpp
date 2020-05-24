#include "registry.h"

#include "main.h"
#include <cstdlib>
#include <cstdio>

const IRQn_Type Nvic::irqn_callback_types_[] = {SysTick_IRQn};
const Gpio::Pin::number_t Nvic::exti_callback_types_[] = {Gpio::Pin::P0};

Button Registry::BTN_USER({BTN_USER_GPIO_Port, BTN_USER_Pin}, 50);
Led Registry::LED_ORANGE({LED_ORANGE_GPIO_Port, LED_ORANGE_Pin});
Led Registry::LED_GREEN({LED_GREEN_GPIO_Port, LED_GREEN_Pin});
Led Registry::LED_RED({LED_RED_GPIO_Port, LED_RED_Pin});
Led Registry::LED_BLUE({LED_BLUE_GPIO_Port, LED_BLUE_Pin});
//FlashLed Registry::LED_ORANGEF({LED_ORANGE_GPIO_Port, LED_ORANGE_Pin}, 500);
//FlashLed Registry::LED_GREENF({LED_GREEN_GPIO_Port, LED_GREEN_Pin}, 500);
//FlashLed Registry::LED_REDF({LED_RED_GPIO_Port, LED_RED_Pin}, 500);
//FlashLed Registry::LED_BLUEF({LED_BLUE_GPIO_Port, LED_BLUE_Pin}, 500);
Uart Registry::UART_2(&huart2, USART2_IRQn);
WatchDog Registry::WDG_I(&hiwdg);

class LedCycle {
protected:
  enum Mode {
    NORMAL,
    REVERSE,
    ALL
  };
  Led *leds_;
  size_t led_count_;
  size_t current_led_;
  Mode current_mode_;
public:
  LedCycle(Led *leds, size_t led_count)
    : leds_(leds), led_count_(led_count), current_led_(0), current_mode_(NORMAL) {
    assert_param(led_count > 1);
  }

  void changeLed() {
    if (this->current_mode_ == ALL) {
      if (this->current_led_ == 0) {
        for (size_t i = 0; i < this->led_count_; i++) {
          this->leds_[i].on();
        }
        this->current_led_ = 1;
      } else {
        for (size_t i = 0; i < this->led_count_; i++) {
          this->leds_[i].off();
        }
        this->current_led_ = 0;
      }
      return;
    }

    this->leds_[this->current_led_].off();
    if (this->current_mode_ == NORMAL) {
      if (++this->current_led_ == this->led_count_) {
        this->current_led_ = 0;
      }
    } else {
      if (this->current_led_ == 0) {
        this->current_led_ = this->led_count_;
      }
      this->current_led_--;
    }
    this->leds_[this->current_led_].on();
  }

  void changeMode(bool *) {
    switch (this->current_mode_) {
      case NORMAL:
        this->current_mode_ = REVERSE;
        break;
      case REVERSE:
        this->current_mode_ = ALL;
        break;
      case ALL:
        this->current_mode_ = NORMAL;
        break;
    }
    //std::exit(0);
  }
};

void uart_echo(Uart::CallbackData *data) {
  if (data->error) {
    Error_Handler();
  }
  Registry::UART_2.send(data->buffer, data->len);
  Registry::UART_2.recv();
}

int main() {
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_IWDG_Init();
  printf("Reset\r\n");

  // Assignment 1.

  // Exercises 7 & 8.
  Led cycle_leds[] = {Registry::LED_BLUE, Registry::LED_GREEN, Registry::LED_ORANGE,
                      Registry::LED_RED};
  LedCycle cycle(cycle_leds, 4);

  Nvic::MemberCallback btn_cb(&LedCycle::changeMode, &cycle);
  Registry::BTN_USER.setCallback(&btn_cb);

  // Exercises 10 & 11.
  char c;
  Nvic::FunctionCallback uart_cb(&uart_echo);
  Registry::UART_2.recv(&c, 1, &uart_cb);

  // Exercise 12.
  Registry::WDG_I.registerRefreshCallback();
  /*Registry::BTN_USER.setPriority(0);
  Nvic::setPriority(SysTick_IRQn, 2);*/

  while (true) {
    cycle.changeLed();
    HAL_Delay(100);
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler() {
  std::exit(1);
}
