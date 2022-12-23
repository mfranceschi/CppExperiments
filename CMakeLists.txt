
# CMake configuration
cmake_minimum_required(VERSION 3.15)
project(MFranceschi_CppExperiments LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 14)

# Add dependencies
include(FetchContent)

FetchContent_Declare(
        MFLib_Fetched
        GIT_REPOSITORY https://github.com/mfranceschi/MFranceschi_CppLibrary.git
        GIT_TAG master
)
FetchContent_MakeAvailable(MFLib_Fetched)
link_libraries(MF_ALL)

add_subdirectory(timing_experience)
