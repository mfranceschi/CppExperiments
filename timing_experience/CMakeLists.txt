add_executable(TimingExperience2 TimingExperience.hpp TimingExperience.cpp ActionToTime.cpp ActionToTime.hpp TimeThis.hpp TimeThis.cpp)
target_link_libraries(TimingExperience2 PRIVATE MF_ALL)


set(EXISTING_FILE_NAME "${CMAKE_HOME_DIRECTORY}/LICENSE")
if (NOT EXISTS ${EXISTING_FILE_NAME})
    message(FATAL_ERROR "File name to read does not exist: ${EXISTING_FILE_NAME}")
endif ()
file(SIZE ${EXISTING_FILE_NAME} EXISTING_FILE_SIZE)

target_compile_definitions(TimingExperience2 PRIVATE EXISTING_FILE_NAME="${EXISTING_FILE_NAME}" EXISTING_FILE_SIZE=${EXISTING_FILE_SIZE} _CRT_SECURE_NO_WARNINGS=1 __STDC_WANT_LIB_EXT1__=1)

if (MINGW)
    set_source_files_properties(
            TimingExperience.cpp
            PROPERTIES
            COMPILE_FLAGS "-Wno-conversion-null"
    )

    if (WIN32)
        target_link_libraries(TimingExperience2 PRIVATE Shlwapi.lib)
    endif ()
endif ()
