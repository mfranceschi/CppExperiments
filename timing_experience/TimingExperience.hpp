//
// Created by Utilisateur on 24/12/2022.
//

#ifndef CPPEXPERIMENTS_TIMINGEXPERIENCE_HPP
#define CPPEXPERIMENTS_TIMINGEXPERIENCE_HPP

#include <cstdlib>

namespace TimingExperience {
    void RunAll();

    void timingTimeThis();

    void timingTheFileExistence();

    void timingTheFileSize();

    void timingWchar_tConversion();

    void timingFileReading();

    void timingCtimeFunctions();

    constexpr std::size_t
    NUMBER_OF_ITERATIONS = 10 * 1000;
} // namespace TimingExperience

#endif //CPPEXPERIMENTS_TIMINGEXPERIENCE_HPP
