//
// Created by MartinF on 31/12/2022.
//

#ifndef MFRANCESCHI_CPPEXPERIMENTS_TIMETHIS_HPP
#define MFRANCESCHI_CPPEXPERIMENTS_TIMETHIS_HPP

#include "ActionToTime.hpp"

/** Returns the execution time for the given function.
 * It runs "iter" times and returns (total_time/iter).
 */
double TimeThis(size_t iter, const Procedure &func);

#endif // MFRANCESCHI_CPPEXPERIMENTS_TIMETHIS_HPP
