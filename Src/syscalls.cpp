#include "registry.h"

extern "C" int _write(int file, char *ptr, int len) {
  Registry::UART_2.send(ptr, len, nullptr);
  return len;
}
