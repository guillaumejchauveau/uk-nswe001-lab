#include "registry.h"

extern "C" int _write(int file, char *ptr, int len) {
  Registry::UART2.send(ptr, len);
  return len;
}
