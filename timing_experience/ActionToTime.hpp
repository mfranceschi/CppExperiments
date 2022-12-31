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

extern const Procedure NO_OP;

struct ActionToTime {
  ActionToTime(const std::string &name, const Procedure &action);

  ActionToTime(const std::string &name, const Procedure &action,
               const Procedure &setUp, const Procedure &tearDown);

  void doRun(std::uint64_t repetitions);

  const std::string name;
  const Procedure action;

  const Procedure setUp = NO_OP;
  const Procedure tearDown = NO_OP;
};

struct ActionWithResultToTime : public ActionToTime {
  ActionWithResultToTime(const std::string &name,
                         const DummyPredicate &predicate);

  ActionWithResultToTime(const std::string &name,
                         const DummyPredicate &predicate,
                         const Procedure &setUp, const Procedure &tearDown);

  const DummyPredicate predicate;
};

#endif // MFRANCESCHI_CPPEXPERIMENTS_ACTIONTOTIME_HPP
