# Not organized

This is a MWP for setting an icon on an executable file.

Objectives:

- (1) When the program is running, in the tasks bar, the icon of the program is our icon.
- (2) (if relevant) We are able to get a reference to the icon.
- (3) On the file explorer, the icon of the program is our icon.

I am using an icon file downloaded from https://icon-icons.com/icon/newton-balls-pendulum-science/233288.

## Windows

### How-to

Create a header file as follows:

```c++
// File MyResource.hpp
#define MY_BEAUTIFUL_ICON 201
```

Create a resource file as follows:

```c++
// File MyResource.rc
#include "MyResource.hpp"
MY_BEAUTIFUL_ICON ICON "relative_path_to_icon.ico"
```

The `.rc` file must be part of the sources of your executable: directly using `target_sources`, or as an `INTERFACE` source file of a target you
are linking with.

If, at some point of your program, you need to get a HICON of your app icon, do this:

```c++
HICON myIcon = static_cast<HICON>(
    LoadImage(
        GetModuleHandle(nullptr), 
        MAKEINTRESOURCE(MY_BEAUTIFUL_ICON),
        IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR
    )
);
if (myIcon == nullptr) {
    // An error occurred from LoadImage. Read below if error is 0x715.
    // TODO: Handle the error
}
// TODO: use the HICON
DestroyIcon(myIcon);
```

### My notes

The icon macro name or numerical value doesn't matter much - except that it may be possible (I haven't tested) that, if
there is >1 ICON in the `.rc` file, the one with the smallest numeric value is used for (1) and (3).

I had a couple of issues with objectives (2) with `LoadImage` and (3). I think that the most important point is that
the `.rc` file is only built and linked directly with the executable we want. If it is linked with an artifact that is
built and then linked to the executable (so typically a static library) then it won't work, even though objective (1) is
okay.

The error I had when working with (2) was `ERROR_RESOURCE_TYPE_NOT_FOUND` = 1813 (0x715). It is sad that this is not
reported directly by the build system (or maybe I have missed it).
