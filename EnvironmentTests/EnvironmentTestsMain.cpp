//
// Created by MartinF on 04/02/2023.
//
#if MF_WINDOWS
#include <Windows.h>
#endif

#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

#if MF_WINDOWS
/**
 * Max number of bytes an environment variable can hold.
 * https://learn.microsoft.com/en-us/windows/win32/api/processenv/nf-processenv-setenvironmentvariablew
 */
static constexpr size_t MAX_LENGTH = 32767;

static std::vector<char> BIG_BUFFER(MAX_LENGTH);
#endif

/**
 * Stores the environment variable into the vector.
 * Returns true on success.
 */
bool get(std::vector<char> &vec, const std::string &name) {
#if MF_WINDOWS
  DWORD result =
      GetEnvironmentVariableA(name.c_str(), vec.data(), vec.capacity());
  if (result == 0) {
    return false;
  } else if (result > vec.capacity()) {
    return GetEnvironmentVariableA(name.c_str(), vec.data(), vec.capacity()) ==
           vec.capacity();
  } else {
    return true;
  }
#else
  const char *result = getenv(name.c_str());
  if (result == nullptr) {
    return false;
  }
  std::strncpy(vec.data(), name.data(), name.length());
  return true;
#endif
}

bool exists(const std::string &name) {
#if MF_WINDOWS
  DWORD result = GetEnvironmentVariableA(name.c_str(), BIG_BUFFER.data(),
                                         BIG_BUFFER.capacity());
  if (result == 0) {
    // Return exists if error is NOT because not found.
    return GetLastError() != ERROR_ENVVAR_NOT_FOUND;
  } else {
    // There is data.
    return true;
  }
#else
  return getenv(name.c_str()) != nullptr;
#endif
}

/**
 * Sets the content of an environment variable.
 * Returns true on success.
 */
bool set(const char *content, const std::string &name) {
#if MF_WINDOWS
  return SetEnvironmentVariableA(name.c_str(), content) == TRUE;
#else
  return setenv(name.c_str(), content, 1) == 0;
#endif
}

void testGlobalVariables() {
  std::string name = "abc123";
  std::vector<char> buffer(999);

  while (exists(name)) {
    name += "aOa";
  }
  std::cout << std::boolalpha << "Using env var named " << name << std::endl << std::endl;

  std::cout << "Setting to \"Eh\"... ";
  assert(set("Eh", name));
  std::cout << "Does it exist? " << exists(name) << std::endl;

  std::cout << "Setting to \"\"... ";
  assert(set("", name));
  std::cout << "Does it exist? " << exists(name) << std::endl << std::endl;

  std::cout << "Setting to \"Eh\"... ";
  assert(set("Eh", name));
  std::cout << "Does it exist? " << exists(name) << std::endl;

  std::cout << "Setting to NULL... ";
  assert(set(nullptr, name));
  std::cout << "Does it exist? " << exists(name) << std::endl;
}

int main() {
  testGlobalVariables();
  return 0;
}
