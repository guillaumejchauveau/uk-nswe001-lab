#include "peripheral/peripherals.h"

extern "C" int _write(int file, char *ptr, int len) {
  peripheral::UART_2.send(ptr, len);
  return len;
}
