#ifndef _LED_H_
#define _LED_H_
#include "tick.h"
#include "gpio.h"

class Led {
protected:
  Gpio::Pin pin_;
public:
  explicit Led(Gpio::Pin pin) : pin_(pin) {
  }

  void on() {
    Gpio::high(this->pin_);
  }

  void off() {
    Gpio::low(this->pin_);
  }
};

class FlashLed : public Led {
protected:
  TickCounter tick_counter_;

public:
  explicit FlashLed(Gpio::Pin pin, size_t ticks) : Led(pin), tick_counter_(ticks,
                                                                           [this](bool *rearm) {
                                                                             this->off();
                                                                             *rearm = false;
                                                                           }) {
  }

  void on() {
    this->tick_counter_.start();
    Led::on();
  }
};

#endif //_LED_H_
