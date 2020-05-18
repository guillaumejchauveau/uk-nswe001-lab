#include "gpio.h"

GPIO_PinState Gpio::read(const Gpio::Pin &pin) {
  return HAL_GPIO_ReadPin(pin.port, pin.number);
}

void Gpio::write(const Gpio::Pin &pin, GPIO_PinState state) {
  return HAL_GPIO_WritePin(pin.port, pin.number, state);
}

void Gpio::high(const Gpio::Pin &pin) {
  Gpio::write(pin, GPIO_PIN_SET);
}

void Gpio::low(const Gpio::Pin &pin) {
  Gpio::write(pin, GPIO_PIN_RESET);
}
