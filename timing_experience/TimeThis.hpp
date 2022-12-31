//
// Created by MartinF on 31/12/2022.
//

#ifndef MFRANCESCHI_CPPEXPERIMENTS_TIMETHIS_HPP
#define MFRANCESCHI_CPPEXPERIMENTS_TIMETHIS_HPP

#include <chrono>
#include <functional>

/** Returns the execution time for the given function.
 * It runs "iter" times and returns (total_time/iter).
 */
inline double TimeThis(size_t iter, const Procedure &func) {
  using namespace std::chrono;
  steady_clock::time_point beginning = steady_clock::now();
  for (size_t i = 0; i < iter; i++) {
    func();
  }
  return (duration<double>(steady_clock::now() - beginning).count()) /
         double(iter);
}

#endif // MFRANCESCHI_CPPEXPERIMENTS_TIMETHIS_HPP
