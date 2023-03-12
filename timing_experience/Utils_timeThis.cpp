//
// Created by Utilisateur on 12/03/2023.
//

#include "Utils.hpp"

#include <chrono>

Ns TimeThis(size_t iter, const Procedure &func) {
    using namespace std::chrono;
    steady_clock::time_point beginning = steady_clock::now();
    for (size_t i = 0; i < iter; i++) {
        func();
    }
    steady_clock::time_point ending = steady_clock::now();

    return duration_cast<Ns>(ending - beginning) /
           iter;
}
