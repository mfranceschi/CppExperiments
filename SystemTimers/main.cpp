//
// Created by MartinF on 18/02/2023.
//

#include "MfTimer.hpp"
#include <array>
#include <iostream>
#include <thread>

static std::atomic<unsigned int> counter{};

int main() {
  constexpr std::size_t ITERATIONS = 99999;
  std::array<std::shared_ptr<MfTimer>, ITERATIONS> pointers{};
  for (int i = 0; i < ITERATIONS; ++i)
    pointers[i] = makeTimer(1500ms, []() { counter++; });

  std::this_thread::sleep_for(2s);
  std::cout << "At this time, " << counter << " increments have been done.";

  return 0;
}
