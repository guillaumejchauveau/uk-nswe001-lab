#ifndef _LED_H_
#define _LED_H_
#include "util/counter.h"
#include "gpio.h"

namespace peripheral {

class Led {
protected:
  Gpio::Pin pin_;
public:
  explicit Led(Gpio::Pin pin) : pin_(pin) {
  }

  void init() {
    Gpio::init(this->pin_, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH);
  }

  void on() {
    Gpio::high(this->pin_);
  }

  void off() {
    Gpio::low(this->pin_);
  }

  void toggle() {
    Gpio::toggle(this->pin_);
  }

  [[nodiscard]] bool isOn() {
    return Gpio::read(this->pin_) == GPIO_PIN_SET;
  }
};

class FlashLed : public Led {
protected:
  Nvic::MemberCallback<bool, FlashLed> tick_counter_callback_;

  void tickCounterCallback(bool *rearm_counter) {
    this->off();
    *rearm_counter = false;
  }

  util::TickCounter tick_counter_;
public:
  explicit FlashLed(Gpio::Pin pin)
    : Led(pin), tick_counter_callback_(&FlashLed::tickCounterCallback, this) {
  }

  void init(uint32_t off_ticks, peripheral::Ticker *ticker) {
    Led::init();
    this->tick_counter_.init(off_ticks, ticker, &this->tick_counter_callback_);
  }

  void on() {
    this->tick_counter_.start();
    Led::on();
  }
};

} // namespace peripheral

#endif //_LED_H_
