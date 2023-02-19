//
// Created by MartinF on 18/02/2023.
//

#ifndef MFRANCESCHI_CPPEXPERIMENTS_MFTIMER_HPP
#define MFRANCESCHI_CPPEXPERIMENTS_MFTIMER_HPP

#include <chrono>
#include <functional>
#include <memory>

using namespace std::chrono_literals;

struct MfTimer {};

template <typename Duration_t>
std::shared_ptr<MfTimer> makeTimer(
    const Duration_t &duration,
    const std::function<void()> &callWhenElapsed = []() {}) {
  return makeTimer(
      std::chrono::duration_cast<std::chrono::nanoseconds>(duration),
      callWhenElapsed);
}

std::shared_ptr<MfTimer> makeTimer(
    const std::chrono::nanoseconds &duration,
    const std::function<void()> &callWhenElapsed = []() {});

#endif // MFRANCESCHI_CPPEXPERIMENTS_MFTIMER_HPP
