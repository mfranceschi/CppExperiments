//
// Created by Utilisateur on 12/03/2023.
//

#include "Utils.hpp"

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
