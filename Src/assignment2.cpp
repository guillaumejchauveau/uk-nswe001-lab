#include "peripheral/peripherals.h"

void task1(bool *) {
  peripheral::LED_GREEN.toggle();
}

void task2(bool *) {
  peripheral::LED_ORANGE.toggle();
}

void task3(bool *) {
  peripheral::LED_RED.toggle();
}

void task4(bool *) {
  peripheral::LED_BLUE.toggle();
}

[[noreturn]] void assignment2() {
  HAL_SYSTICK_Config(SystemCoreClock / (1000U / 50));
  peripheral::LED_BLUE.init();
  peripheral::LED_GREEN.init();
  peripheral::LED_ORANGE.init();
  peripheral::LED_RED.init();

  peripheral::Nvic::FunctionCallback<bool> task1Cb(&task1);
  peripheral::Nvic::FunctionCallback<bool> task2Cb(&task2);
  peripheral::Nvic::FunctionCallback<bool> task3Cb(&task3);
  peripheral::Nvic::FunctionCallback<bool> task4Cb(&task4);

  // Exercise 1.
  util::TickCounter task1Counter(&task1Cb, 2);
  util::TickCounter task2Counter(&task2Cb, 3);
  util::TickCounter task3Counter(&task3Cb, 5);
  util::TickCounter task4Counter(&task4Cb, 8);
  task1Counter.start();
  task2Counter.start();
  task3Counter.start();
  task4Counter.start();

  while (true) {
  }
}
