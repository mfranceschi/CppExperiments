add_executable(TimingExperience2 TimingExperience.hpp TimingExperience.cpp)
target_link_libraries(TimingExperience2 PRIVATE MF_ALL)


set(FILE_NAME "${CMAKE_HOME_DIRECTORY}/LICENSE")
if (NOT EXISTS ${FILE_NAME})
    message(FATAL_ERROR "File name to read does not exist: ${FILE_NAME}")
endif()

target_compile_definitions(TimingExperience2 PRIVATE FILE_NAME="${FILE_NAME}")

if(MINGW)
    set_source_files_properties(
            TimingExperience.cpp
            PROPERTIES
            COMPILE_FLAGS "-Wno-conversion-null"
    )

    if(WIN32)
        target_link_libraries(TimingExperience2 PRIVATE Shlwapi.lib)
    endif()
endif()
