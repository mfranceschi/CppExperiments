//
// Created by MartinF on 18/02/2023.
//

#include "MfTimer.hpp"
#include <array>
#include <iostream>
#include <thread>

int main() {
  constexpr std::size_t ITERATIONS = 999;
  std::array<std::shared_ptr<MfTimer>, ITERATIONS> pointers{};
  for (int i = 0; i < ITERATIONS; ++i)
    pointers[i] =
        makeTimer(1s, []() { std::cout << "Done waiting 2s" << std::endl; });

  std::this_thread::sleep_for(4s);

  return 0;
}
