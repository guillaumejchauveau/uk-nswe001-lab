#include "peripheral/peripherals.h"

class LedCycle {
protected:
  enum Mode {
    NORMAL,
    REVERSE,
    ALL
  };
  peripheral::Led *leds_;
  uint8_t led_count_;
  uint8_t current_led_;
  Mode current_mode_;
public:
  LedCycle(peripheral::Led *leds, uint8_t led_count)
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

void uart_echo(peripheral::Uart::CallbackData *data) {
  if (data->error) {
    Error_Handler();
  }
  peripheral::UART_2.send(data->buffer, data->len);
  peripheral::UART_2.recv();
}

[[noreturn]] void watch_dog_loop(bool *) {
  while (true) {

  }
}

[[noreturn]] void assignment1() {
  peripheral::LED_BLUE.init();
  peripheral::LED_GREEN.init();
  peripheral::LED_ORANGE.init();
  peripheral::LED_RED.init();
  // Exercises 7 & 8.
  peripheral::Led cycle_leds[] = {peripheral::LED_BLUE, peripheral::LED_GREEN,
                                  peripheral::LED_ORANGE,
                                  peripheral::LED_RED};
  LedCycle cycle(cycle_leds, 4);

  peripheral::Nvic::MemberCallback btn_cb(&LedCycle::changeMode, &cycle);
  peripheral::BTN_USER.init(&btn_cb, 50, &peripheral::TICKER_SYSTICK, 2);

  // Exercises 10 & 11.
  char c;
  peripheral::Nvic::FunctionCallback uart_cb(&uart_echo);
  peripheral::UART_2.recv(&c, 1, &uart_cb);

  // Exercise 12.
  /*Peripheral::WDG_I.init(
    {
      .Prescaler = IWDG_PRESCALER_4,
      .Reload = 4095
    });
  Peripheral::Nvic::FunctionCallback wdg_loop_cb(&watch_dog_loop);
  Peripheral::BTN_USER.init(&wdg_loop_cb, 50, 0);
  Peripheral::Nvic::setPriority(SysTick_IRQn, 2);*/

  while (true) {
    cycle.changeLed();
    HAL_Delay(100);
  }
}
