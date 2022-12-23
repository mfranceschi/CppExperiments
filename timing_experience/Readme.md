# Introduction

During the creation process of the MFranceschi_CppLibrary, I wanted to ensure that my code ran as fast as possible.

For instance, there are several ways of getting a file size in Windows:

* You can get the position of a stream (`FILE*` or `fstream`) after positioning it at the end of the file.
* You can use system calls to retrieve the file attribute: the portable `fstat`, the Windows-only
  brother `GetFileInformationEx`.
* You can use a system call to retrieve the file size directly: the Windows-only `GetFileSizeEx`.
* Probably other functions that I don't know the existence yet.

Four different ways for getting a file size, all of them take from one to four lines, and take no longer than a few
microseconds to run. But **which one is the fastest**? This simple question was my goal. I also wanted to ensure that my
observations were great enough to be considered acceptable.

In this document, I will present the results I obtained. Please note that all timing results are taken from the same run
of the script. Of course, please feel free to run it by yourself. Normally CMake should have generated an executable
with the `Shlwapi.dll` dependency.

# Results

Result of the timing function, without anything to run (empty lambda function): `3.3187e-07` seconds. It can be
considered as a default offset.

## Check if, given a filepath, the file exists

I have two constants that hold the same file path as C-strings, one of them being a `const char*` and the other
a `const wchar_t*`. All the functions called have to write into a shared integer variable the boolean return.

1. [Windows-only] `PathFileExists`: 2.06946e-05 seconds.
2. `stat`: 3.40244e-05 seconds.
3. [Windows-only]: `GetFileAttributes`: 1.72648e-05 seconds.

For a request as simple as checking a file existence, where no other information should be gathered, seeing that (3) is
faster that (1) is quite surprising.

---

UNPOLISHED

```
The duration of the execution of 'TimeThis' itself, without anything to do, is: 3.3187e-07

Timing the file existence functions !
Time for PathFileExists: 2.06946e-05
Time for stat: 3.40244e-05
Time for GetFileAttributes: 1.72648e-05

Timing the file size functions !
Time for fseek : 3.92838e-05
Time for GetFileAttributesEx: 1.73985e-05
Time for GetFileSizeEx: 2.93431e-05

Timing char to wchar_t conversion functions!
Time for mbstowcs_s: 5.1667e-07
Time for wstring creation: 7.57953e-06

Timing functions for reading 5 chars in a file!
Time for FILE* with fgets: 4.22333e-05
Time for FILE* with fgetc: 3.84125e-05
Time for ifstream with getline: 4.47927e-05
Time for ifstream with get: 5.11807e-05
Time for Windows syscalls: 3.99883e-05

Timing <ctime> conversion functions between tm and time_t !
Time for localtime: 8.4769e-07
Time for localtime_s: 7.0153e-07
Time for mktime (with local [FR] time tm): 1.66125e-06
Time for gmtime: 4.3435e-07
Time for gmtime_s: 4.6011e-07
Time for mktime (with GMT time tm): 1.69234e-06
```

# Sources:

* http://erikerlandson.github.io/blog/2012/03/19/interaction-between-mktime-and-tm-isdst-a-compute-cycle-landmine/
* https://stackoverflow.com/questions/8558919/mktime-and-tm-isdst
