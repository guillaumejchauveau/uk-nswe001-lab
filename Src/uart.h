#ifndef _UART_H_
#define _UART_H_
#include <utility>

#include "nvic.h"
#include "main.h"

class Uart {
public:
  struct CallbackData {
    char *buffer;
    int len;
    bool error;
  };
protected:
  UART_HandleTypeDef *handle_;
  IRQn_Type irqn_;
  Nvic::callback_t<CallbackData> tx_callback_;
  char *rx_buffer_;
  int rx_len_;
  Nvic::callback_t<CallbackData> rx_callback_;
public:
  Uart(UART_HandleTypeDef *handle, IRQn_Type IRQn)
    : handle_(handle), irqn_(IRQn), rx_buffer_(nullptr), rx_len_(0) {
  }

  void setPriority(uint32_t PreemptPriority, uint32_t SubPriority = 0) {
    Nvic::setPriority(this->irqn_, PreemptPriority, SubPriority);
  }

  HAL_StatusTypeDef send(char *buffer, int len) {
    return HAL_UART_Transmit_IT(this->handle_, reinterpret_cast<uint8_t *>(buffer), len);
  }

  HAL_StatusTypeDef send(char *buffer, int len, Nvic::callback_t<CallbackData> callback) {
    this->tx_callback_ = std::move(callback);
    return this->send(buffer, len);
  }

  HAL_StatusTypeDef recv() {
    return HAL_UART_Receive_IT(this->handle_, reinterpret_cast<uint8_t *>(this->rx_buffer_),
                               this->rx_len_);
  }

  HAL_StatusTypeDef recv(char *buffer, int len) {
    this->rx_buffer_ = buffer;
    this->rx_len_ = len;
    return this->recv();
  }

  HAL_StatusTypeDef recv(char *buffer, int len, Nvic::callback_t<CallbackData> callback) {
    this->rx_buffer_ = buffer;
    this->rx_len_ = len;
    this->rx_callback_ = std::move(callback);
    return this->recv();
  }

  void _handleTxCplt() {
    if (!this->tx_callback_) {
      return;
    }
    CallbackData data{
      .buffer = nullptr,
      .len = 0,
      .error = false
    };
    this->tx_callback_.operator()(&data);
  }

  void _handleRxCplt() {
    if (!this->rx_callback_) {
      return;
    }
    CallbackData data{
      .buffer = this->rx_buffer_,
      .len = this->rx_len_,
      .error = false
    };
    this->rx_callback_.operator()(&data);
  }

  void _handleError() {
    if (!this->tx_callback_ && !this->rx_callback_) {
      Error_Handler();
    }

    CallbackData data{
      .buffer = nullptr,
      .len = 0,
      .error = true
    };

    if (this->tx_callback_) {
      this->tx_callback_.operator()(&data);
    }
    if (this->rx_callback_) {
      this->rx_callback_.operator()(&data);
    }
  }
};

#endif //_UART_H_
