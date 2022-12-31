//
// Created by MartinF on 31/12/2022.
//

#include "ActionToTime.hpp"
#include "TimeThis.hpp"
#include <cassert>
#include <iostream>

using namespace std;

void ActionToTime::doRun(std::uint64_t repetitions) {
  cout << "Time for " << this->name << ": " << TimeThis(repetitions, action)
       << endl;
}

ActionToTime::ActionToTime(const string &name, const Procedure &action)
    : name(name), action(action) {}

ActionWithResultToTime::ActionWithResultToTime(const string &name,
                                               const DummyPredicate &predicate)
    : ActionToTime(name, [&predicate]() { assert(predicate()); }) {}
