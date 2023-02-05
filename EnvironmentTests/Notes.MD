# Tests with environment variables

## Experiment

Measure the behaviour of the function to set the value of an environment variable.

Process:

1. Determine a name for an environment variable that does not exist.
2. Set it a non-empty value and ensure the environment variable exists.
3. Set it to `""` and check if the environment variable exists.
4. Set it a non-empty value and ensure the environment variable exists.
5. Set it to `NULL` and check if the environment variable exists.

## Results

| Platform | Set to ""    | Set to NULL   |
|----------|--------------|---------------|
| Windows  | Unset (1)    | Unset (1)     |
| Debian   | Still exists | Seg fault (2) |

(1) The fact that NULL un-sets / deletes the variable is actually documented
for [the SetEnvironmentVariable function](https://learn.microsoft.com/en-us/windows/win32/api/processenv/nf-processenv-setenvironmentvariablea).
What is NOT explained is that setting to an empty string actually deletes the variable as well. I could not find many
infos around it, but the C# .NET platform has the same implementation detail as can be seen
on [this GitHub issue](https://github.com/dotnet/runtime/issues/50554). (Edit: it is
actually [documented here](https://learn.microsoft.com/fr-fr/dotnet/api/system.environment.setenvironmentvariable?view=net-7.0).)

(2) On Debian, the segmentation fault happens because `setenv` actually _makes a copy_ of the C-string passed by
pointer. It's not just keeping the pointer, it's copying the contents. When debugging, I saw that it called `strlen` (an
assembly version of it). According to POSIX that is the by-design behaviour, even if it's not stated on many Linux
documentations. See [POSIX v2004](https://pubs.opengroup.org/onlinepubs/009604499/functions/setenv.html) (also valid
under [POSIX v2017](https://pubs.opengroup.org/onlinepubs/9699919799/functions/setenv.html)).

### Thoughts and conclusions

- Lack of documentation, or unclear function names, is very error-prone.
- Inconsistencies between platform is heavily annoying.
- I think that wrapper writers should abstract these differences away and specify their own unified API, that would
  follow Windows' convention but also include a function
  like `unset` for completeness.
- The segfault is an unacceptable output for such as easy-to-check case.
