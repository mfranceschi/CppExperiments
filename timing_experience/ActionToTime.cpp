//
// Created by MartinF on 31/12/2022.
//

#include "ActionToTime.hpp"
#include "TimeThis.hpp"
#include <cassert>
#include <iostream>

using namespace std;

const Procedure NO_OP = Procedure([]() {});

void ActionToTime::doRun(std::uint64_t repetitions) {
  this->setUp();
  cout << "Time for " << this->name << ": " << TimeThis(repetitions, action)
       << endl;
  this->tearDown();
}

ActionToTime::ActionToTime(const string &name, const Procedure &action)
    : name(name), action(action) {}

ActionToTime::ActionToTime(const string &name, const Procedure &action,
                           const Procedure &setUp, const Procedure &tearDown)
    : name(name), action(action), setUp(setUp), tearDown(tearDown) {}

ActionWithResultToTime::ActionWithResultToTime(const string &name,
                                               const DummyPredicate &predicate)
    : ActionToTime(name, [&predicate]() { assert(predicate()); }) {}

ActionWithResultToTime::ActionWithResultToTime(const string &name,
                                               const DummyPredicate &predicate,
                                               const Procedure &setUp,
                                               const Procedure &tearDown)
    : ActionToTime(
          name, [&predicate]() { assert(predicate()); }, setUp, tearDown) {}
