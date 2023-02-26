//
// Created by MartinF on 18/02/2023.
//

#include "MfTimer.hpp"
#include <array>
#include <atomic>
#include <iostream>
#include <thread>

/**
 * How many timers are we creating?
 */
constexpr std::size_t NBR_TIMERS = 7500;

/**
 * Callbacks shall increment this.
 * It starts at 0.
 * At the end of the <code>main</code> function, it is expected to be equal to <code>NBR_TIMERS</code>.
 */
static std::atomic<unsigned int> counter{};

int main() {
    std::array<std::shared_ptr<MfTimer>, NBR_TIMERS> pointers{};
    for (int i = 0; i < NBR_TIMERS; ++i) {
        pointers[i] = makeTimer(1500ms, []() { counter++; });
    }

    // Just to check data.
    std::cout << "Period=" << pointers[0]->getPeriod().count() << ", time left="
              << std::chrono::duration_cast<std::chrono::milliseconds>(
                      pointers[0]->getTimeBeforeNextExpiration())
                      .count()
              << " ms" << std::endl;

    // The timers should sleep 1,5s.
    // This is testing how many timers have called the callback
    // and incremented the atomic counter after 2s.
    std::this_thread::sleep_for(2s);
    std::cout << "At this time, " << counter << " increments (out of "
              << NBR_TIMERS << ") have been done." << std::endl;

    return EXIT_SUCCESS;
}
