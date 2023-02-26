
add_executable(MfSystemTimers MfTimer.hpp main.cpp MfTimer_Windows.cpp MfTimer_Debian.cpp)

find_library(LIBRT rt)
if (LIBRT)
    target_link_libraries(MfSystemTimers ${LIBRT})
endif ()
