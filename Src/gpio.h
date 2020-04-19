#ifndef _GPIO_H_
#define _GPIO_H_

#include "main.h"
#include <initializer_list>
#include <cstdlib>

namespace GPIO {
struct Pin {
  enum Number {
    P0 = GPIO_PIN_0,
    P1 = GPIO_PIN_1,
    P2 = GPIO_PIN_2,
    P3 = GPIO_PIN_3,
    P4 = GPIO_PIN_4,
    P5 = GPIO_PIN_5,
    P6 = GPIO_PIN_6,
    P7 = GPIO_PIN_7,
    P8 = GPIO_PIN_8,
    P9 = GPIO_PIN_9,
    P10 = GPIO_PIN_10,
    P11 = GPIO_PIN_11,
    P12 = GPIO_PIN_12,
    P13 = GPIO_PIN_13,
    P14 = GPIO_PIN_14,
    P15 = GPIO_PIN_15,
    P_All = GPIO_PIN_All
  };

  typedef GPIO_TypeDef *port_t;
  typedef Number number_t;

  static constexpr std::initializer_list<number_t> pin_numbers = {
    P0, P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12, P13, P14, P15
  };

  static number_t convert_number(uint16_t raw_number) {
    assert_param(IS_GPIO_PIN(raw_number));
    for (auto number : pin_numbers) {
      if (number == raw_number) {
        return number;
      }
    }
    std::exit(1);
  }

  port_t port;
  number_t number;

  Pin(GPIO_TypeDef *port, uint16_t pin) : port(port), number(GPIO::Pin::convert_number(pin)) {

  }
};

GPIO_PinState read(const GPIO::Pin &pin);

void write(const GPIO::Pin &pin, GPIO_PinState state);

void high(const GPIO::Pin &pin);

void low(const GPIO::Pin &pin);
}

#endif //_GPIO_H_
