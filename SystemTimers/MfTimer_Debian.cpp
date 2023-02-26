//
// Created by Utilisateur on 23/02/2023.
//

#if MF_UNIX

#include "MfTimer.hpp"
#include <atomic>
#include <csignal>
#include <cstring>
#include <ctime>
#include <iostream>
#include <map>

static const clockid_t MY_CLOCK = CLOCK_MONOTONIC;

static std::atomic<int> currentIndex{};

static std::map<int, std::function<void()>> indexToFunctions{};

static void myCallback(sigval value) {
    std::cout << __func__ << std::endl;
    int functionIndex = value.sival_int;
    indexToFunctions[functionIndex]();
}

struct MfTimer_Unix : MfTimer {
    /** ID of timer to delete on destruction. Never checked for validity.*/
    const timer_t timerId;

    explicit MfTimer_Unix(timer_t timerId) : timerId(timerId) {}

    /**
     * Returns the time left before the next expiration.
     * Returns zero if timer is not armed.
     */
    std::chrono::nanoseconds getTimeBeforeNextExpiration() const override {
        itimerspec myTimerSpec;
        timer_gettime(timerId, &myTimerSpec);
        return std::chrono::seconds(myTimerSpec.it_value.tv_sec) +
               std::chrono::nanoseconds(myTimerSpec.it_value.tv_nsec);
    }

    /// Returns zero if not periodic or if not armed.
    std::chrono::nanoseconds getPeriod() const override {
        itimerspec myTimerSpec;
        timer_gettime(timerId, &myTimerSpec);
        return std::chrono::seconds(myTimerSpec.it_interval.tv_sec) +
               std::chrono::nanoseconds(myTimerSpec.it_interval.tv_nsec);
    }

    ~MfTimer_Unix() override { timer_delete(timerId); }
};

std::shared_ptr<MfTimer>
makeTimerWithNanos(const std::chrono::nanoseconds &duration,
                   const std::function<void()> &callWhenElapsed) {
    int myFunctionIndex = currentIndex++;

    sigevent_t sigevent{0};
    sigevent.sigev_notify = SIGEV_THREAD;
    sigevent.sigev_value.sival_int = myFunctionIndex;
    sigevent.sigev_notify_function = &myCallback;

    timer_t myTimer;
    int result = timer_create(MY_CLOCK, &sigevent, &myTimer);
    if (result) {
        const auto cachedErrno = errno;
        std::cout << "timer_create failed! errno=" << cachedErrno << " - "
                  << strerror(cachedErrno) << std::endl;
        ::exit(42);
    }

    itimerspec myTimerSpec{0};
    myTimerSpec.it_value.tv_sec = duration.count() / 1000000000;
    myTimerSpec.it_value.tv_nsec = duration.count() % 1000000000;
    result = timer_settime(myTimer, 0, &myTimerSpec, nullptr);
    if (result) {
        const auto cachedErrno = errno;
        std::cout << "timer_create failed! errno=" << cachedErrno << " - "
                  << strerror(cachedErrno) << std::endl;
        ::exit(42);
    }

    indexToFunctions.emplace(myFunctionIndex, callWhenElapsed);

    return std::make_shared<MfTimer_Unix>(myTimer);
}

#endif
