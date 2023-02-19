# Investigations on OS-provided timer features

## Windows

### SetTimer

The Win32 [SetTimer](https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-settimer) function creates a timer, and we can specify a callback function. I haven't had any major difficulty making it work - at least for the timer creation part. What was weird is that my callback function (that printed on `stdout` "Hello, World!") didn't seem to have been called.

[By digging more into how it is supposed to work](https://stackoverflow.com/a/15685291), the thing is that it is not supposed to directly call the callback function. When the timer elapses, it adds a `MSG` to the message queue, and when doing `TranslateMessage` then `DispatchMessage`, only at that time will it call the callback function.

In other words, this API is only meant to work with apps that have a message queue, which is not what I am looking for.

## Timer-queue timers

TODO...
- ref to the fact that [it works for non-service apps](https://learn.microsoft.com/en-us/windows/win32/api/threadpoollegacyapiset/)
- need for own timer queue or use default timer queue?
