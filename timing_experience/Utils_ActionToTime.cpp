//
// Created by Utilisateur on 12/03/2023.
//

#include "Utils.hpp"

#include <cassert>
#include <iostream>
#include <utility>
#include <vector>
#include <algorithm>

using namespace std;

using Pair = std::pair<std::string, Ns>;
static std::vector<Pair> latestActions{};

void ActionToTime::doRun(std::uint64_t repetitions) {
    Ns dur = TimeThis(repetitions, action);
    cout << "Time for " << this->name << ": " << dur.count() << "ns"
         << endl;

    latestActions.emplace_back(name, dur);
}

ActionToTime::ActionToTime(string name, Procedure action)
        : name(std::move(name)), action(std::move(action)) {}

ActionWithResultToTime::ActionWithResultToTime(const string &name,
                                               const DummyPredicate &predicate)
        : ActionToTime(name, [&predicate]() { assert(predicate()); }) {}

std::pair<std::string, Ns> getFastestAction() {
    auto myIt = std::min_element(latestActions.cbegin(), latestActions.cend(),
                                 [](const Pair &pair1, const Pair &pair2) {
                                     return pair1.second < pair2.second;
                                 });
    std::pair<std::string, Ns> myCopy = *myIt;
    latestActions.clear();
    return myCopy;
}

std::ostream &operator<<(std::ostream &theStream, const Pair &toPrint) {
    theStream << "The fastest action is " << toPrint.first << " with ";

    if (toPrint.second.count() > 10'000) {
        auto dur2 = std::chrono::duration_cast<std::chrono::microseconds>(toPrint.second);
        theStream << dur2.count() << " microseconds.";
    } else {
        theStream << toPrint.second.count() << " nanoseconds.";
    }
    return theStream << std::endl;
}
