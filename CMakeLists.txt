cmake_minimum_required(VERSION 3.15)
project(MFranceschi_CppExperiments LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 14)

add_subdirectory(cmake)

link_libraries(MF_ALL MF_SystemErrors)

add_subdirectory(timing_experience)
add_subdirectory(AppWithIcon)
add_subdirectory(EnvironmentTests)
add_subdirectory(SystemTimers)
