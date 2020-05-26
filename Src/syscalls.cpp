#include "peripheral/peripherals.h"
#include "mutex.h"

Mutex writeMutex("_write");

extern "C" int _write(int file, char *ptr, int len) {
  writeMutex.acquire();
  peripheral::UART_2.send(ptr, len);
  writeMutex.release();
  return len;
}
