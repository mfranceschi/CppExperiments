//
// Created by Utilisateur on 12/03/2023.
//

#ifndef MFRANCESCHI_CPPEXPERIMENTS_UTILS_HPP
#define MFRANCESCHI_CPPEXPERIMENTS_UTILS_HPP

#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <memory>
#include <string>

using PtrFILE = std::unique_ptr<std::FILE, decltype(&std::fclose)>;

using Procedure = std::function<void(void)>;
using DummyPredicate = std::function<bool(void)>;

using Ns = std::chrono::nanoseconds;

constexpr std::size_t NUMBER_OF_ITERATIONS = 10 * 1000;

/**
 * An action to run and time.
 */
struct ActionToTime {
    ActionToTime(std::string name, Procedure action);

    void doRun(std::uint64_t repetitions = NUMBER_OF_ITERATIONS);

    const std::string name;
    const Procedure action;
};

/**
 * An action to run and time.
 * The action is a function that returns a boolean which shall be always true.
 */
struct ActionWithResultToTime : public ActionToTime {
    ActionWithResultToTime(const std::string &name,
                           const DummyPredicate &predicate);

    const DummyPredicate predicate;
};

/**
 * After running several actions, this will return the fastest one based on mean runtime.
 * It internally reads an array that contains the latest results, then empties it.
 */
std::pair<std::string, Ns> getFastestAction();

std::ostream & operator<< (std::ostream& theStream, const std::pair<std::string, Ns>& toPrint);

/** Returns the execution time for the given function.
 * It runs "iter" times and returns (total_time/iter).
 */
Ns TimeThis(size_t iter, const Procedure &func);

/** Returns the same C-string but converted in wchar_t*.
 * It is created using new[] --> please use delete[] after use!
 * Returns nullptr in case of failure.
 */
wchar_t *ToWchar_t(const char *source);

#endif //MFRANCESCHI_CPPEXPERIMENTS_UTILS_HPP
