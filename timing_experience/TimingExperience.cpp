#include "TimingExperience.hpp"

#include <sys/stat.h>

#include <array>
#include <chrono>
#include <cstring>
#include <ctime>
#include <functional>
#include <iostream>
#include <string>

#include "ActionToTime.hpp"
#include "MF/Filesystem.hpp"
#include "TimeThis.hpp"

#if MF_WINDOWS
#include <Shlwapi.h>
#include <fcntl.h>
#include <io.h>
#endif

#ifdef _MSC_VER
#define stat _stat
#pragma comment(lib, "Shlwapi.lib")
#endif

using std::cout;
using std::endl;

static inline double TimeWithRepetition(const std::function<void()> &func) {
  return TimeThis(TimingExperience::NUMBER_OF_ITERATIONS, func);
}

// Returns the same C-string but converted in wchar_t*.
// It is created using new[] --> please use delete[] after use!
// Returns nullptr in case of failure.
static inline wchar_t *ToWchar_t(const char *source) {
  size_t length = strlen(source);
  std::unique_ptr<wchar_t[]> destination =
      std::make_unique<wchar_t[]>(length + 1);
  size_t retValue = 0;

#if defined _MSC_VER ||                                                        \
    (defined __STDC_LIB_EXT1__ && defined __STDC_WANT_LIB_EXT1__ &&            \
     __STDC_WANT_LIB_EXT1__ == 1)
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
  ActionToTime("TimeThis", []() {}).doRun(NUMBER_OF_ITERATIONS);
  cout << endl;
}

void timingTheFileExistence() {
  cout << "Timing the file existence functions !" << endl;

  ActionWithResultToTime("stat", []() {
    struct stat statStruct {};
    return stat(EXISTING_FILE_NAME, &statStruct) == 0;
  }).doRun(NUMBER_OF_ITERATIONS);

#if MF_WINDOWS
  ActionWithResultToTime("PathFileExistsA", []() {
    return PathFileExistsA(EXISTING_FILE_NAME);
  }).doRun(NUMBER_OF_ITERATIONS);

  ActionWithResultToTime("GetFileAttributesA", []() {
    return GetFileAttributesA(EXISTING_FILE_NAME) != INVALID_FILE_ATTRIBUTES;
  }).doRun(NUMBER_OF_ITERATIONS);
#endif // WIN32

  cout << endl;
}

void timingTheFileSize() {
  cout << "Timing the file size functions !" << endl;

  ActionWithResultToTime("fopen then fseek", []() {
    const auto theFile = std::fopen(EXISTING_FILE_NAME, "r");
    fseek(theFile, 0, SEEK_END);
    const auto theSize = ftell(theFile);
    return (fclose(theFile) == 0) && (theSize == EXISTING_FILE_SIZE);
  }).doRun(NUMBER_OF_ITERATIONS);

  ActionWithResultToTime("stat", []() {
    struct stat statStruct {};
    stat(EXISTING_FILE_NAME, &statStruct);
    return statStruct.st_size == EXISTING_FILE_SIZE;
  }).doRun(NUMBER_OF_ITERATIONS);

#if MF_WINDOWS

  ActionWithResultToTime("GetFileAttributesExA", []() {
    WIN32_FILE_ATTRIBUTE_DATA fileInfo;
    GetFileAttributesExA(EXISTING_FILE_NAME, GetFileExInfoStandard,
                         (void *)&fileInfo);
    return ULARGE_INTEGER{fileInfo.nFileSizeLow, fileInfo.nFileSizeHigh}
               .QuadPart == EXISTING_FILE_SIZE;
  }).doRun(NUMBER_OF_ITERATIONS);

  ActionWithResultToTime("GetFileSizeEx", []() {
    HANDLE file =
        CreateFileA(EXISTING_FILE_NAME, GENERIC_READ, FILE_SHARE_WRITE, nullptr,
                    OPEN_EXISTING, NULL, nullptr);
    LARGE_INTEGER res;
    GetFileSizeEx(file, &res);
    CloseHandle(file);
    return res.QuadPart == EXISTING_FILE_SIZE;
  }).doRun(NUMBER_OF_ITERATIONS);
#endif // WIN32

  cout << endl;
}

void timingWchar_tConversion() {
  cout << "Timing char to wchar_t conversion functions!" << endl;
  const std::string input = EXISTING_FILE_NAME;
  const std::wstring expected(input.cbegin(), input.cend());

  ActionWithResultToTime("mbstowcs_s", [&]() {
    wchar_t *result = ToWchar_t(EXISTING_FILE_NAME);
    bool isOkay = expected == result;
    delete[] result;
    return isOkay;
  }).doRun(NUMBER_OF_ITERATIONS);

  ActionWithResultToTime("wstring constructor", [&]() {
    return std::wstring(input.cbegin(), input.cend()) == expected;
  }).doRun(NUMBER_OF_ITERATIONS);

  cout << endl;
}

void timingFileReading() {
  cout << "Timing functions for reading 5 chars in a file!" << endl;
  static constexpr std::size_t BUFFER_SIZE = 6;
  static_assert(BUFFER_SIZE < EXISTING_FILE_SIZE,
                "We won't be able to read 5 chars "
                "since the EXISTING_FILE_SIZE is smaller.");
  std::array<char, BUFFER_SIZE> buffer{};

  ActionToTime("FILE* with fgets", [&]() {
    PtrFILE theFile(std::fopen(EXISTING_FILE_NAME, "r"), std::fclose);
    fgets(buffer.data(), BUFFER_SIZE, theFile.get());
  }).doRun(NUMBER_OF_ITERATIONS);

  ActionToTime("FILE* with fgetc", [&]() {
    PtrFILE theFile(std::fopen(EXISTING_FILE_NAME, "r"), std::fclose);
    for (std::size_t i = 0; i < BUFFER_SIZE; ++i) {
      buffer.at(i) = static_cast<char>(fgetc(theFile.get()));
    }
    buffer[BUFFER_SIZE - 1] = '\0';
  }).doRun(NUMBER_OF_ITERATIONS);

  ActionToTime("ifstream with getline", [&]() {
    std::ifstream ifs(EXISTING_FILE_NAME);
    ifs.getline(buffer.data(), BUFFER_SIZE);
  }).doRun(NUMBER_OF_ITERATIONS);

  ActionToTime("ifstream with get", [&]() {
    std::ifstream ifs(EXISTING_FILE_NAME);
    for (int i = 0; i < BUFFER_SIZE; ++i) {
      buffer.at(i) = static_cast<char>(ifs.get());
    }
    buffer[BUFFER_SIZE - 1] = '\0';
  }).doRun(NUMBER_OF_ITERATIONS);

#if defined _MSC_VER
  ActionToTime("ifstream with getline", [&]() {
    int file;
    _sopen_s(&file, EXISTING_FILE_NAME, _O_RDONLY, _SH_DENYNO, _S_IREAD);
    _read(file, buffer.data(), BUFFER_SIZE - 1);
    _close(file);
  }).doRun(NUMBER_OF_ITERATIONS);
#endif // _MSC_VER

  cout << endl;
}

void timingCtimeFunctions() {
  cout << "Timing <ctime> conversion functions between tm and time_t !" << endl;
  time_t timestamp = time(nullptr);
  tm tmStruct{};

  ActionToTime("localtime", [&]() {
    tmStruct = *localtime(&timestamp);
  }).doRun(NUMBER_OF_ITERATIONS);

#if MF_WINDOWS || defined(__STDC_LIB_EXT1__) && __STDC_WANT_LIB_EXT1__ == 1
  ActionToTime("localtime_s", [&]() {
    localtime_s(&tmStruct, &timestamp);
  }).doRun(NUMBER_OF_ITERATIONS);

  ActionToTime("gmtime_s", [&]() {
    gmtime_s(&tmStruct, &timestamp);
  }).doRun(NUMBER_OF_ITERATIONS);
#endif

  ActionToTime("mktime (using local time) in struct tm", [&]() {
    timestamp = mktime(&tmStruct);
  }).doRun(NUMBER_OF_ITERATIONS);

  ActionToTime("gmtime", [&]() {
    tmStruct = *gmtime(&timestamp);
  }).doRun(NUMBER_OF_ITERATIONS);

  ActionToTime("mktime (using local time) in struct tm", [&]() {
    timestamp = mktime(&tmStruct);
  }).doRun(NUMBER_OF_ITERATIONS);
}
} // namespace TimingExperience

int main() {
  TimingExperience::RunAll();
  return 0;
}
