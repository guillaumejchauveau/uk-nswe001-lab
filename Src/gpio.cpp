#include "gpio.h"

GPIO_PinState GPIO::read(const GPIO::Pin &pin) {
  return HAL_GPIO_ReadPin(pin.port, pin.number);
}

void GPIO::write(const GPIO::Pin &pin, GPIO_PinState state) {
  return HAL_GPIO_WritePin(pin.port, pin.number, state);
}

void GPIO::high(const GPIO::Pin &pin) {
  GPIO::write(pin, GPIO_PIN_SET);
}

void GPIO::low(const GPIO::Pin &pin) {
  GPIO::write(pin, GPIO_PIN_RESET);
}
