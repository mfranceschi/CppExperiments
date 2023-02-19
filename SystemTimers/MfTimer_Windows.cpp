//
// Created by MartinF on 18/02/2023.
//

#if MF_WINDOWS

#include "MfTimer.hpp"
#include <Windows.h>
#include <mutex>
#include <stdexcept>

/**
 * Set <code>API_TO_USE</code> to this value to use
 * the <code>SetTimer</code> API.
 */
#define API_TO_USE_SET_TIMER 1

/**
 * Set <code>API_TO_USE</code> to this value to use
 * the <code>CreateTimerQueueTimer</code> API.
 */
#define API_TO_USE_TIMER_QUEUE_TIMER 2

#ifndef API_TO_USE
/**
 * Change the value to one of the above to set which API family to use,
 * The value set is compile-checked.
 */
#define API_TO_USE API_TO_USE_TIMER_QUEUE_TIMER
#endif

#if API_TO_USE == API_TO_USE_SET_TIMER
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

#elif API_TO_USE == API_TO_USE_TIMER_QUEUE_TIMER

/**
 * Contains the function to call.
 * The destructor releases the timer HANDLE, if any.
 */
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

/**
 * Unique callback for all timers.
 * @param lpParam Pointer to MfTimer_Windows*
 * @param TimerOrWaitFired
 */
void CALLBACK myTimerRoutine(PVOID lpParam, BOOLEAN TimerOrWaitFired) {
  (void)TimerOrWaitFired;

  // Note: in theory we cannot end up in a situation
  // where the callback is called
  // and the timer has been deleted = the destructor has been called == lpParam
  // points to some already-freed memory.
  auto *mfTimerWindows = static_cast<MfTimer_Windows *>(lpParam);
  mfTimerWindows->callWhenElapsed();
}

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

  HANDLE returnedHandle;
  HANDLE timerQueue = nullptr;
  auto parameters = timerPtr.get();
  DWORD dueTime =
      std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
  DWORD period = 0;
  ULONG flags = WT_EXECUTEONLYONCE;

  BOOL success =
      CreateTimerQueueTimer(&returnedHandle, timerQueue, myTimerRoutine,
                            parameters, dueTime, period, flags);
  if (!success) {
    throw std::runtime_error("Error!");
  }
  timerPtr->timerHandle = returnedHandle;

  return timerPtr;
}
#else
#error Please use one of the allowed API families!
#endif

#endif
