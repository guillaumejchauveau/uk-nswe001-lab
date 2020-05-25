#include "peripheral/peripherals.h"

void task1(void *) {
  peripheral::LED_GREEN.toggle();
}

void task2(void *) {
  peripheral::LED_ORANGE.toggle();
}

void task3(void *) {
  peripheral::LED_RED.toggle();
}

void task4(void *) {
  peripheral::LED_BLUE.toggle();
}

[[noreturn]] void assignment2() {
  HAL_SYSTICK_Config(SystemCoreClock / (1000U / 50));
  peripheral::LED_BLUE.init();
  peripheral::LED_GREEN.init();
  peripheral::LED_ORANGE.init();
  peripheral::LED_RED.init();

  peripheral::Nvic::FunctionCallback<void> task1Cb(&task1);
  peripheral::Nvic::FunctionCallback<void> task2Cb(&task2);
  peripheral::Nvic::FunctionCallback<void> task3Cb(&task3);
  peripheral::Nvic::FunctionCallback<void> task4Cb(&task4);

  // Exercise 1.
  /*util::TickCounter task1Counter(2, &peripheral::TICKER_SYSTICK, &task1Cb);
  util::TickCounter task2Counter(3, &peripheral::TICKER_SYSTICK, &task2Cb);
  util::TickCounter task3Counter(5, &peripheral::TICKER_SYSTICK, &task3Cb);
  util::TickCounter task4Counter(8, &peripheral::TICKER_SYSTICK, &task4Cb);
  task1Counter.start();
  task2Counter.start();
  task3Counter.start();
  task4Counter.start();*/

  // Exercise 2.
  TIM_Base_InitTypeDef tim_init{};
  tim_init.CounterMode = TIM_COUNTERMODE_UP;
  tim_init.ClockDivision = TIM_CLOCKDIVISION_DIV1;

  tim_init.Prescaler = 64000;
  tim_init.Period = 25;
  peripheral::TICKER_TIM2.init(tim_init, 3);
  peripheral::TICKER_TIM2.onTick(&task1Cb);
  tim_init.Prescaler = 60000;
  tim_init.Period = 40;
  peripheral::TICKER_TIM3.init(tim_init, 4);
  peripheral::TICKER_TIM3.onTick(&task2Cb);
  tim_init.Prescaler = 50000;
  tim_init.Period = 80;
  peripheral::TICKER_TIM4.init(tim_init, 5);
  peripheral::TICKER_TIM4.onTick(&task3Cb);
  tim_init.Prescaler = 64000;
  tim_init.Period = 100;
  peripheral::TICKER_TIM5.init(tim_init, 6);
  peripheral::TICKER_TIM5.onTick(&task4Cb);

  while (true) {
  }
}
