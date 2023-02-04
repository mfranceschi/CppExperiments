//
// Created by MartinF on 29/01/2023.
//

#if MF_WINDOWS
#include "MF/LightWindows.hpp"
#include "MF/SystemErrors.hpp"
#include "My_Resource.hpp"
#include <iostream>

int main() {
  try {
    MF::SystemErrors::Win32::throwCurrentSystemErrorIf(
        LoadImage(GetModuleHandle(nullptr), MAKEINTRESOURCE(MY_BEAUTIFUL_ICON),
                  IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR) == nullptr);
  } catch (const MF::SystemErrors::SystemError &systemError) {
    std::cerr << "System error! 0x" << std::hex << systemError.getErrorCode()
              << " - " << systemError.what() << std::endl;
    // return 1;
  }

  // MessageBox is just a quick way to have a taskbar icon with our exe,
  // and without some console wrapper that could hide the real icon.
  MessageBox(nullptr, "Info, this is a pointless text.", "Not an information",
             MB_ICONQUESTION | MB_OK | MB_DEFBUTTON2);
  return 0;
}
#endif
