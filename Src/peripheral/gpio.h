#ifndef _GPIO_H_
#define _GPIO_H_

#include "main.h"

namespace peripheral {

class Gpio {
public:
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

    static constexpr number_t pin_numbers[] = {
      P0, P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12, P13, P14, P15
    };

    static number_t convert_number(uint16_t raw_number) {
      assert_param(IS_GPIO_PIN(raw_number));
      for (auto pin_number : pin_numbers) {
        if (pin_number == raw_number) {
          return pin_number;
        }
      }
      Error_Handler();
    }

    port_t port;
    number_t number;

    Pin(GPIO_TypeDef *port, uint16_t pin) : port(port), number(Gpio::Pin::convert_number(pin)) {

    }
  };

  static void init(const Gpio::Pin &pin, uint32_t mode, uint32_t pull,
                   uint32_t speed = GPIO_SPEED_FREQ_LOW,
                   uint32_t alternate = 0) {

    GPIO_InitTypeDef GPIO_InitStruct = {
      .Pin = pin.number,
      .Mode = mode,
      .Pull = pull,
      .Speed = speed,
      .Alternate = alternate
    };
    HAL_GPIO_Init(pin.port, &GPIO_InitStruct);
  }

  static GPIO_PinState read(const Gpio::Pin &pin) {
    return HAL_GPIO_ReadPin(pin.port, pin.number);
  }

  static void write(const Gpio::Pin &pin, GPIO_PinState state) {
    HAL_GPIO_WritePin(pin.port, pin.number, state);
  }

  static void toggle(const Gpio::Pin &pin) {
    HAL_GPIO_TogglePin(pin.port, pin.number);
  }

  static void high(const Gpio::Pin &pin) {
    Gpio::write(pin, GPIO_PIN_SET);
  }

  static void low(const Gpio::Pin &pin) {
    Gpio::write(pin, GPIO_PIN_RESET);
  }
};

} // namespace peripheral

#endif //_GPIO_H_
