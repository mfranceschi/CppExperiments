# I am adding '2' at the end of the executable name because of a naming clash.
add_executable(TimingExperience2 Utils.hpp Utils_timeThis.cpp Utils_ActionToTime.cpp Utils_toWchar_t.cpp main.cpp)
target_link_libraries(TimingExperience2 PRIVATE MF_ALL)

# Grab a file that will exist and be accessible at runtime.
set(EXISTING_FILE_NAME "${CMAKE_HOME_DIRECTORY}/LICENSE")
if (NOT EXISTS ${EXISTING_FILE_NAME})
    message(FATAL_ERROR "File name to read does not exist: ${EXISTING_FILE_NAME}")
endif ()
file(SIZE ${EXISTING_FILE_NAME} EXISTING_FILE_SIZE)

target_compile_definitions(TimingExperience2
    PRIVATE
        EXISTING_FILE_NAME="${EXISTING_FILE_NAME}"
        EXISTING_FILE_SIZE=${EXISTING_FILE_SIZE}

        # Windows-specific
        _CRT_SECURE_NO_WARNINGS=1

        # *nix-specific
        __STDC_WANT_LIB_EXT1__=1
        )

if (MINGW)
    set_source_files_properties(
            ARCHIVE_TimingExperience.cpp
            PROPERTIES
            COMPILE_FLAGS "-Wno-conversion-null"
    )

    if (WIN32)
        # target_link_libraries(TimingExperience2 PRIVATE Shlwapi.lib)
    endif ()
endif ()
