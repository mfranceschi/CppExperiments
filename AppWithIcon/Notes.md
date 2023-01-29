# Not organized

This is a MWP for setting an icon on an executable file.

## Windows

As part of your exe sources, include a .rc file that contains the following line:

```c++
MY_ICON_NAME ICON 1 "relative_path_to_icon.ico"
```

The ICON with the smallest ID will be used as your app icon on the task bar when running.

If you also want to have that icon visible in the Windows explorer, TODO.
