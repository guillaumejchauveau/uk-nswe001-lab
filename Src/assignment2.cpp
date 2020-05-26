#include <cstdio>
#include "peripheral/peripherals.h"

void task1(void *) { //bool
  peripheral::LED_GREEN.toggle();
}

void task2(void *) { //bool
  peripheral::LED_ORANGE.toggle();
}

void task3(void *) { //bool
  peripheral::LED_RED.toggle();
}

void task4(void *) { //bool
  peripheral::LED_BLUE.toggle();
}

[[noreturn]] void assignment2() {
  peripheral::LED_BLUE.init();
  peripheral::LED_GREEN.init();
  peripheral::LED_ORANGE.init();
  peripheral::LED_RED.init();

  peripheral::Nvic::FunctionCallback<void> task1Cb(&task1); //bool
  peripheral::Nvic::FunctionCallback<void> task2Cb(&task2); //bool
  peripheral::Nvic::FunctionCallback<void> task3Cb(&task3); //bool
  peripheral::Nvic::FunctionCallback<void> task4Cb(&task4); //bool

  // Exercise 1.
  /*SystemCoreClockUpdate();
  HAL_SYSTICK_Config(HAL_RCC_GetSysClockFreq() / (1000U / 50));
  util::TickCounter task1Counter(2, &peripheral::TICKER_SYSTICK, &task1Cb);
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

  auto clock_freq = HAL_RCC_GetPCLK1Freq();

  tim_init.Prescaler = (uint16_t) (clock_freq / 10 / 25 - 1);
  tim_init.Period = 25;
  peripheral::TICKER_TIM2.init(tim_init, 3);
  peripheral::TICKER_TIM2.onTick(&task1Cb);

  tim_init.Prescaler = (uint16_t) (clock_freq / 20 * 3 / 40 - 1);
  tim_init.Period = 40;
  peripheral::TICKER_TIM3.init(tim_init, 4);
  peripheral::TICKER_TIM3.onTick(&task2Cb);

  tim_init.Prescaler = (uint16_t) (clock_freq / 4 / 64 - 1);
  tim_init.Period = 64;
  peripheral::TICKER_TIM4.init(tim_init, 5);
  peripheral::TICKER_TIM4.onTick(&task3Cb);

  tim_init.Prescaler = (uint16_t) (clock_freq / 5 * 2 / 100 - 1);
  tim_init.Period = 100;
  peripheral::TICKER_TIM5.init(tim_init, 6);
  peripheral::TICKER_TIM5.onTick(&task4Cb);

  while (true) {
  }
}
