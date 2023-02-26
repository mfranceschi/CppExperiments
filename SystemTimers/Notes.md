# Investigations on OS-provided timer features

## Windows

### SetTimer

The Win32 [SetTimer](https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-settimer) function creates a timer, and we can specify a callback function. I haven't had any major difficulty making it work - at least for the timer creation part. What was weird is that my callback function (that printed on `stdout` "Hello, World!") didn't seem to have been called.

[By digging more into how it is supposed to work](https://stackoverflow.com/a/15685291), the thing is that it is not supposed to directly call the callback function. When the timer elapses, it adds a `MSG` to the message queue, and when doing `TranslateMessage` then `DispatchMessage`, only at that time will it call the callback function.

In other words, this API is only meant to work with apps that have a message queue, which is not what I am looking for.

### Timer-queue timers

I have found a few different APIs for creating timers depending on what kind of app it is and how it wants to interact with the timer object. [This page lists 4 of them](https://www.codeproject.com/Articles/1236/Timers-Tutorial).

The one that I have found to be working and fitting my need is the "Timer-queue timer". The idea is: you create a collection of timers which is called a **Timer Queue** (note, this is optional), and you can add **Timers** to this timer queue. Features:
- It will actually call the callback function once the timer elapses!
- It can be used also for timers that are periodic, using the optional `period` parameter when creating the timer.
- It is described as lightweight.
- The delay and the period of a specific timer [can be changed after creation](https://learn.microsoft.com/en-us/windows/win32/api/threadpoollegacyapiset/nf-threadpoollegacyapiset-changetimerqueuetimer), as long as it is not considered as having finished its task (such as a one-shot timer that is done running). 
- API resolution is in milliseconds.

My findings:
- It seems to be part of a legacy API, according to the [header name](https://learn.microsoft.com/en-us/windows/win32/api/threadpoollegacyapiset/). It was introduced with Windows 2000 (the `_WIN32_WINNT` is the one of [Windows 2000](https://learn.microsoft.com/en-us/cpp/porting/modifying-winver-and-win32-winnt)). This could mean that it will be deprecated or removed at some point, however the documentation doesn't mention anything like that.
- The doc says that [it is used by system services](https://learn.microsoft.com/en-us/windows/win32/api/threadpoollegacyapiset/), but it does not seem to be a requirement, since I could make it work easily with a minimal console program.
- Using the default timer queue (I mean, not creating your own and passing `nullptr` as the timer queue parameter) works as well. I think it is useful if you want to have the ability to delete all your timers at once easily: just [delete your timer queue](https://learn.microsoft.com/en-us/windows/win32/api/threadpoollegacyapiset/nf-threadpoollegacyapiset-deletetimerqueueex).

## Linux

### POSIX timers

- Clocks: many variants, depend on POSIX and Linux versions. Not all clocks have the same features implemented; the only ones that seem safe at all levels are `CLOCK_REALTIME` and `CLOCK_MONOTONIC`.
- Compile with `-lrt` for `librt`, the POSIX.1b Realtime Extensions library.
- Errno 95 = `timer_create` is not supported on the platform or compiler OR it does not work with the specified `clockid_t`. Confusing. Example: `timer_create` was not implemented at all on WSL at some point
- Tests on my Linux setups:
  - Ubuntu 22.04.2
    - The limit seems to be between 65000 and 65535 timers existing at the same time. My understanding is that [timers are mapped to kernel objects](https://codebrowser.dev/glibc/glibc/sysdeps/unix/sysv/linux/timer_create.c.html), so there is a limited number of timers the kernel can work with at a given time for all processes. 
    - When using more than 10^4 timers, many callbacks were not called in a reasonable time. A hypothesis can be in relation with a bottleneck with mutexes.
  - Ubuntu on WSL2 on Windows 10 22H2
    - The limit seems to be between 7500 and 8000 timers existing at the same time.
    - On all of my various attempts, only a tiny amount of callback functions were called at all, maybe around 1/50th. I suppose the implementation is not exhaustive. This sounds dangerous for apps that heavily rely on accurate timers.
