//
// Created by MartinF on 31/12/2022.
//

#ifndef MFRANCESCHI_CPPEXPERIMENTS_ACTIONTOTIME_HPP
#define MFRANCESCHI_CPPEXPERIMENTS_ACTIONTOTIME_HPP

#include <cstdint>
#include <functional>
#include <string>

using Procedure = std::function<void(void)>;
using DummyPredicate = std::function<bool(void)>;

/**
 * An action to run and time.
 */
struct ActionToTime {
  ActionToTime(const std::string &name, const Procedure &action);

  void doRun(std::uint64_t repetitions);

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

#endif // MFRANCESCHI_CPPEXPERIMENTS_ACTIONTOTIME_HPP
