#ifndef _UART_H_
#define _UART_H_

#include "nvic.h"
#include "main.h"

namespace peripheral {

class Uart {
public:
  struct CallbackData {
    char *buffer;
    uint16_t len;
    bool error;
  };
protected:
  UART_HandleTypeDef handle_{};
  IRQn_Type irqn_;
  Gpio::Pin tx_pin_;
  Gpio::Pin rx_pin_;
  uint32_t uart_gpio_alternate_function_;
  Nvic::Callback <CallbackData> *tx_user_callback_{};
  char *rx_buffer_{};
  uint16_t rx_len_{};
  Nvic::Callback <CallbackData> *rx_user_callback_{};
public:
  Uart(USART_TypeDef *instance, IRQn_Type IRQn,
       Gpio::Pin txPin, Gpio::Pin rxPin, uint32_t uartGpioAlternateFunction)
    : irqn_(IRQn), tx_pin_(txPin), rx_pin_(rxPin),
      uart_gpio_alternate_function_(uartGpioAlternateFunction) {
    this->handle_.Instance = instance;
  }

  void init(UART_InitTypeDef init, uint32_t PreemptPriority, uint32_t SubPriority = 0) {
    Gpio::init(this->rx_pin_, GPIO_MODE_AF_PP, GPIO_PULLUP, GPIO_SPEED_FREQ_VERY_HIGH,
               this->uart_gpio_alternate_function_);
    Gpio::init(this->tx_pin_, GPIO_MODE_AF_PP, GPIO_PULLUP, GPIO_SPEED_FREQ_VERY_HIGH,
               this->uart_gpio_alternate_function_);
    Nvic::setPriority(this->irqn_, PreemptPriority, SubPriority);
    Nvic::enable(this->irqn_);

    this->handle_.Init = init;
    if (HAL_UART_Init(&this->handle_) != HAL_OK) {
      Error_Handler();
    }
  }

  HAL_StatusTypeDef send(char *buffer, uint16_t len) {
    return HAL_UART_Transmit_IT(&this->handle_, reinterpret_cast<uint8_t *>(buffer), len);
  }

  HAL_StatusTypeDef send(char *buffer, uint16_t len, Nvic::Callback <CallbackData> *callback) {
    this->tx_user_callback_ = callback;
    return this->send(buffer, len);
  }

  HAL_StatusTypeDef recv() {
    return HAL_UART_Receive_IT(&this->handle_, reinterpret_cast<uint8_t *>(this->rx_buffer_),
                               this->rx_len_);
  }

  HAL_StatusTypeDef recv(char *buffer, uint16_t len) {
    this->rx_buffer_ = buffer;
    this->rx_len_ = len;
    return this->recv();
  }

  HAL_StatusTypeDef recv(char *buffer, uint16_t len, Nvic::Callback <CallbackData> *callback) {
    this->rx_buffer_ = buffer;
    this->rx_len_ = len;
    this->rx_user_callback_ = callback;
    return this->recv();
  }

  UART_HandleTypeDef *getHandle() {
    return &this->handle_;
  }

  void _handleTxCplt() {
    if (!this->tx_user_callback_) {
      return;
    }
    CallbackData data{
      .buffer = nullptr,
      .len = 0,
      .error = false
    };
    this->tx_user_callback_->operator()(&data);
  }

  void _handleRxCplt() {
    if (!this->rx_user_callback_) {
      return;
    }
    CallbackData data{
      .buffer = this->rx_buffer_,
      .len = this->rx_len_,
      .error = false
    };
    this->rx_user_callback_->operator()(&data);
  }

  void _handleError() {
    if (!this->tx_user_callback_ && !this->rx_user_callback_) {
      Error_Handler();
    }

    CallbackData data{
      .buffer = nullptr,
      .len = 0,
      .error = true
    };

    if (this->tx_user_callback_) {
      this->tx_user_callback_->operator()(&data);
    }
    if (this->rx_user_callback_) {
      this->rx_user_callback_->operator()(&data);
    }
  }
};

} // namespace Peripheral

#endif //_UART_H_
