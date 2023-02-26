//
// Created by MartinF on 18/02/2023.
//

#ifndef MFRANCESCHI_CPPEXPERIMENTS_MFTIMER_HPP
#define MFRANCESCHI_CPPEXPERIMENTS_MFTIMER_HPP

#include <chrono>
#include <functional>
#include <iostream>
#include <memory>


using namespace std::chrono_literals;

struct MfTimer {
    virtual std::chrono::nanoseconds getTimeBeforeNextExpiration() const = 0;

    virtual std::chrono::nanoseconds getPeriod() const = 0;

    virtual ~MfTimer() = default;
};

std::shared_ptr<MfTimer>
makeTimerWithNanos(const std::chrono::nanoseconds &duration,
                   const std::function<void()> &callWhenElapsed);

template<typename Duration_t>
std::shared_ptr<MfTimer>
makeTimer(const Duration_t &duration,
          const std::function<void()> &callWhenElapsed) {
    return makeTimerWithNanos(
            std::chrono::duration_cast<std::chrono::nanoseconds>(duration),
            callWhenElapsed);
}

#endif // MFRANCESCHI_CPPEXPERIMENTS_MFTIMER_HPP
