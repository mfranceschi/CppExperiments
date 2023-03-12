//
// Created by Utilisateur on 12/03/2023.
//

#include "Utils.hpp"

#include <chrono>

double TimeThis(size_t iter, const Procedure &func) {
    using namespace std::chrono;
    steady_clock::time_point beginning = steady_clock::now();
    for (size_t i = 0; i < iter; i++) {
        func();
    }
    return (duration<double>(steady_clock::now() - beginning).count()) /
           double(iter);
}
