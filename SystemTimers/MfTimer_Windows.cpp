//
// Created by MartinF on 18/02/2023.
//

#if MF_WINDOWS

#include "MfTimer.hpp"
#include <Windows.h>
#include <atomic>
#include <iostream>
#include <mutex>
#include <stdexcept>
#include <unordered_map>

/*
static std::unordered_map<UINT, std::function<void()>> timerIdsToCallbacks{};

static void __stdcall myTimerProc(HWND windowHandle, UINT message, UINT timerId,
                                  DWORD systemTime) {
  (void)windowHandle;
  (void)message;
  (void)systemTime;

  const auto foundItem = timerIdsToCallbacks.find(timerId);
  if (foundItem != timerIdsToCallbacks.cend()) {
    foundItem->second();
  }
}

std::shared_ptr<MfTimer>
makeTimer(const std::chrono::nanoseconds &duration,
          const std::function<void()> &callWhenElapsed) {
  UINT elapse =
      std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
  UINT timerId =
      SetTimer(nullptr, 0, elapse, static_cast<TIMERPROC>(myTimerProc));

  if (timerId == 0) {
    throw std::runtime_error("Error!");
  }
  timerIdsToCallbacks.insert(std::make_pair(timerId, callWhenElapsed));
  return std::make_shared<MfTimer>();
}
 */

/*
static std::unordered_map<unsigned int, std::function<void()>>
    timerIdsToCallbacks{};

void CALLBACK myTimerRoutine(PVOID lpParam, BOOLEAN TimerOrWaitFired) {
  std::cout << __func__ << std::endl;
  (void)TimerOrWaitFired;

  const auto foundItem = timerIdsToCallbacks.find((unsigned int)lpParam);
  if (foundItem != timerIdsToCallbacks.cend()) {
    foundItem->second();
  }
}
 */

void CALLBACK myTimerRoutine(PVOID lpParam, BOOLEAN TimerOrWaitFired) {
  std::cout << __func__ << std::endl;
}

static std::atomic<unsigned int> atomicInt{};

struct MfTimer_Windows : MfTimer {
  const std::function<void()> callWhenElapsed;

  explicit MfTimer_Windows(const std::function<void()> &callWhenElapsed)
      : callWhenElapsed(callWhenElapsed){};

  ~MfTimer_Windows() {
    if (timerHandle != nullptr) {
      DeleteTimerQueueTimer(nullptr, timerHandle, nullptr);
    }
  }

  HANDLE timerHandle = nullptr;
};

static HANDLE myTimerQueue = nullptr;
static std::once_flag onceFlag{};

std::shared_ptr<MfTimer>
makeTimer(const std::chrono::nanoseconds &duration,
          const std::function<void()> &callWhenElapsed) {
  std::call_once(
      onceFlag,
      [](HANDLE *timerQueueRef) { *timerQueueRef = CreateTimerQueue(); },
      &myTimerQueue);

  auto timerPtr = std::make_shared<MfTimer_Windows>(callWhenElapsed);

  /*
  auto timerId = atomicInt++;
  timerIdsToCallbacks.insert(std::make_pair(timerId, callWhenElapsed));
   */

  HANDLE returnedHandle;
  HANDLE timerQueue = nullptr;
  auto parameters = timerPtr.get();
  DWORD dueTime =
      std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
  DWORD period = 0;
  ULONG flags =  WT_EXECUTEONLYONCE;

  BOOL success =
      CreateTimerQueueTimer(&returnedHandle, timerQueue, myTimerRoutine,
                            parameters, dueTime, period, flags);
  if (!success) {
    throw std::runtime_error("Error!");
  }
  timerPtr->timerHandle = returnedHandle;

  return timerPtr;
}

#endif
