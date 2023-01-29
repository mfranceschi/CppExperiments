
add_library(MyIconLib INTERFACE)
target_include_directories(MyIconLib INTERFACE icon)
target_sources(MyIconLib INTERFACE icon/My_Resource.rc)

add_executable(AppWithResource_Exec code/TheMainForAppWithResource.cpp)
target_link_libraries(AppWithResource_Exec PRIVATE MyIconLib)
