# Tests with environment variables

Results:

| Platform | Set to ""    | Set to NULL   |
|----------|--------------|---------------|
| Windows  | Unset (1)    | Unset (1)     |
| Debian   | Still exists | Seg fault (2) |

(1) The fact that NULL un-sets / deletes the variable is actually documented for [the SetEnvironmentVariable function](https://learn.microsoft.com/en-us/windows/win32/api/processenv/nf-processenv-setenvironmentvariablea). What is NOT explained is that setting to an empty string actually deletes the variable as well. I could not find many infos around it, but the C# .NET platform has the same undocumented implementation detail as can be seen on [this GitHub issue](https://github.com/dotnet/runtime/issues/50554).

(2) On Debian, the segmentation fault happens because `setenv` actually _makes a copy_ of the C-string passed by pointer. It's not just keeping the pointer, it's copying the contents. When debugging, I saw that it called `strlen` (an assembly version of it). According to POSIX that is the by-design behaviour, even if it's not stated on many Linux documentations. See [POSIX v2004](https://pubs.opengroup.org/onlinepubs/009604499/functions/setenv.html) (also valid under [POSIX v2017](https://pubs.opengroup.org/onlinepubs/9699919799/functions/setenv.html)).
