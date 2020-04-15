#include "stm32f4xx.h"

#define RAMLOG_LENGTH 2048
char ramlog[RAMLOG_LENGTH];
int ramlogIndex = 0;

extern "C" int _write(int file, char *ptr, int len) {
  int index;
  if (!ptr) {
    return 0;
  }
  for (index = 0; index < len; index++) {

    ramlog[ramlogIndex] = ptr[index];
    ramlogIndex++;
    if (ramlogIndex >= RAMLOG_LENGTH) {
      ramlogIndex = 0;
    }

    ITM_SendChar(
      (uint8_t) ptr[index]); // Sends it to ST-Link SWO as well so that it can be observed in ST-Link Utility
  }
  return len;
}
