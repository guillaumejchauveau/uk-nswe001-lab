#ifndef _LED_H_
#define _LED_H_

class Led {
protected:
  GPIO_TypeDef *GPIOx;
  uint16_t GPIO_Pin;
public:
  Led(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) : GPIOx(GPIOx), GPIO_Pin(GPIO_Pin) {
  }

  void on() {
    HAL_GPIO_WritePin(this->GPIOx, this->GPIO_Pin, GPIO_PIN_SET);
  }

  void off() {
    HAL_GPIO_WritePin(this->GPIOx, this->GPIO_Pin, GPIO_PIN_RESET);
  }
};

#endif //_LED_H_
