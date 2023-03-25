
add_library(MyLib INTERFACE)
target_sources(MyLib INTERFACE MyLib/MyLib.cpp)
target_include_directories(MyLib INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/MyLib")

add_executable(Exec1 Exec1/Exec1.cpp)
target_link_libraries(Exec1 MyLib)

add_executable(Exec2 Exec2/Exec2.cpp)
target_link_libraries(Exec2 MyLib)
