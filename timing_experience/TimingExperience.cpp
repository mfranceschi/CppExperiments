#include "TimingExperience.hpp"

#include <sys/stat.h>

#include <array>
#include <chrono>
#include <cstring>
#include <ctime>
#include <functional>
#include <iostream>
#include <string>

#include "MF/Filesystem.hpp"

#if MF_WINDOWS
#    include <Shlwapi.h>
#    include <fcntl.h>
#    include <io.h>
#endif

#ifdef _MSC_VER
#    define stat _stat
#    pragma comment(lib, "Shlwapi.lib")
#endif

using std::cout;
using std::endl;

// Returns the execution time for the given function.
// It runs "iter" times and returns (total_time/iter).
static inline double TimeThis(size_t iter, const std::function<void(void)> &func) {
    using namespace std::chrono;
    high_resolution_clock::time_point beginning = high_resolution_clock::now();
    for (size_t i = 0; i < iter; i++) {
        func();
    }
    return (duration<double>(high_resolution_clock::now() - beginning).count()) / double(iter);
}

static inline double TimeWithRepetition(const std::function<void()> &func) {
    return TimeThis(TimingExperience::NUMBER_OF_ITERATIONS, func);
}

// Returns the same C-string but converted in wchar_t*.
// It is created using new[] --> please use delete[] after use!
// Returns nullptr in case of failure.
static inline wchar_t *ToWchar_t(const char *source) {
    size_t length = strlen(source);
    std::unique_ptr<wchar_t[]> destination = std::make_unique<wchar_t[]>(length + 1);
    size_t retValue = 0;

#if defined _MSC_VER || \
    (defined __STDC_LIB_EXT1__ && defined __STDC_WANT_LIB_EXT1__ && __STDC_WANT_LIB_EXT1__ == 1)
    mbstowcs_s(&retValue, destination.get(), length + 1, source, length);
#else
    retValue = mbstowcs(destination.get(), source, length) - length;
#endif

    if (retValue == 0U) {
        destination.reset();
    }
    return destination.release();
}

using PtrFILE = std::unique_ptr<std::FILE, decltype(&std::fclose)>;

namespace TimingExperience {
    void RunAll() {
        timingTimeThis();
        timingTheFileExistence();
        timingTheFileSize();
        timingWchar_tConversion();
        timingFileReading();
        timingCtimeFunctions();
    }

    void timingTimeThis() {
        cout << "The duration of the execution of 'TimeThis' itself, without anything to do, is: ";
        cout << TimeWithRepetition([]() {
        }) << endl;
        cout << endl;
    }

    void timingTheFileExistence() {
        cout << "Timing the file existence functions !" << endl;

        cout << "Time for stat: " << TimeWithRepetition([]() {
            struct stat statStruct{};
            stat(FILE_NAME, &statStruct);
        }) << endl;

#if MF_WINDOWS
        cout << "Time for PathFileExists: ";
        cout << TimeWithRepetition([]() {
            PathFileExists(file_L);
        }) << endl;

        cout << "Time for GetFileAttributes: ";
        cout << TimeWithRepetition([]() {
            DWORD attr = GetFileAttributes(file_L);
        }) << endl;
#endif // WIN32

        cout << endl;
    }

    void timingTheFileSize() {
        cout << "Timing the file size functions !" << endl;

        cout << "Time for fseek : ";
        cout << TimeWithRepetition([]() {
#pragma warning(suppress : 4996)
            PtrFILE theFile(std::fopen(FILE_NAME, "r"), std::fclose);
            fseek(theFile.get(), 0, SEEK_END);
            ftell(theFile.get());
        }) << endl;

        cout << "Time for stat: ";
        cout << TimeWithRepetition([]() {
            struct stat statStruct{};
            stat(FILE_NAME, &statStruct);
        }) << endl;

#if MF_WINDOWS
        cout << "Time for GetFileAttributesEx: ";
        cout << TimeWithRepetition([]() {
            WIN32_FILE_ATTRIBUTE_DATA fileInfo;
            GetFileAttributesEx(file_L, GetFileExInfoStandard, (void *)&fileInfo);
        }) << endl;

        cout << "Time for GetFileSizeEx: ";
        cout << TimeWithRepetition([]() {
            HANDLE file = CreateFile(
                file_L, GENERIC_READ, FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, NULL, nullptr);
            LARGE_INTEGER res;
            GetFileSizeEx(file, &res);
            CloseHandle(file);
        }) << endl;
#endif // WIN32

        cout << endl;
    }

    void timingWchar_tConversion() {
        cout << "Timing char to wchar_t conversion functions!" << endl;

        cout << "Time for mbstowcs_s: ";
        cout << TimeWithRepetition([]() {
            std::unique_ptr<wchar_t[]> newptr(ToWchar_t(FILE_NAME));
        }) << endl;

        cout << "Time for wstring creation: ";
        cout << TimeWithRepetition([]() {
            std::string temp = FILE_NAME;
            std::wstring wideTemp = std::wstring(temp.cbegin(), temp.cend());
        }) << endl;

        cout << endl;
    }

    void timingFileReading() {
#pragma warning(disable : 4996)
        cout << "Timing functions for reading 5 chars in a file!" << endl;
        static constexpr std::size_t BUFFER_SIZE = 6;
        std::array<char, BUFFER_SIZE> buffer{};

        cout << "Time for FILE* with fgets: ";
        cout << TimeWithRepetition([&buffer]() {
            PtrFILE theFile(std::fopen(FILE_NAME, "r"), std::fclose);
            fgets(buffer.data(), BUFFER_SIZE, theFile.get());
        }) << endl;

        cout << "Time for FILE* with fgetc: ";
        cout << TimeWithRepetition([&buffer]() {
            PtrFILE theFile(std::fopen(FILE_NAME, "r"), std::fclose);
            for (std::size_t i = 0; i < BUFFER_SIZE; ++i) {
                buffer.at(i) = static_cast<char>(fgetc(theFile.get()));
            }
            buffer[BUFFER_SIZE - 1] = '\0';
        }) << endl;

        cout << "Time for ifstream with getline: ";
        cout << TimeWithRepetition([&buffer]() {
            std::ifstream ifs(FILE_NAME);
            ifs.getline(buffer.data(), BUFFER_SIZE);
            ifs.close();
        }) << endl;

        cout << "Time for ifstream with get: ";
        cout << TimeWithRepetition([&buffer]() {
            std::ifstream ifs(FILE_NAME);
            for (int i = 0; i < BUFFER_SIZE; ++i) {
                buffer.at(i) = static_cast<char>(ifs.get());
            }
            buffer[BUFFER_SIZE - 1] = '\0';
            ifs.close();
        }) << endl;

#if defined _MSC_VER
        cout << "Time for Windows syscalls: ";
        cout << TimeWithRepetition([&buffer]() {
            int file;
            _sopen_s(&file, file_name, _O_RDONLY, _SH_DENYNO, _S_IREAD);
            auto tmp = _read(file, buffer.data(), BUFFER_SIZE - 1);
            _close(file);
        }) << endl;
#endif // _MSC_VER

        cout << endl;
#pragma warning(default : 4996)
    }

    void timingCtimeFunctions() {
#pragma warning(disable : 4996)
        cout << "Timing <ctime> conversion functions between tm and time_t !" << endl;
        time_t timet = time(nullptr);
        tm tmt{};

        cout << "Time for localtime: ";
        cout << TimeWithRepetition([&timet, &tmt]() {
            tmt = *localtime(&timet);
        }) << endl;

#if defined __STDC_LIB_EXT1__ && __STDC_WANT_LIB_EXT1__ == 1
        cout << "Time for localtime_s: ";
        cout << TimeWithRepetition([&timet, &tmt]() {
            localtime_s(&tmt, &timet);
        }) << endl;

        cout << "Time for gmtime_s: ";
        cout << TimeWithRepetition([&timet, &tmt]() {
            gmtime_s(&tmt, &timet);
        }) << endl;
#endif

        cout << "Time for mktime (with local time tm): ";
        cout << TimeWithRepetition([&timet, &tmt]() {
            timet = mktime(&tmt);
        }) << endl;

        cout << "Time for gmtime: ";
        cout << TimeWithRepetition([&timet, &tmt]() {
            tmt = *gmtime(&timet);
        }) << endl;

        cout << "Time for mktime (with GMT time tm): ";
        cout << TimeWithRepetition([&timet, &tmt]() {
            timet = mktime(&tmt);
        }) << endl;
    }
} // namespace TimingExperience

int main() {
    TimingExperience::RunAll();
    return 0;
}
