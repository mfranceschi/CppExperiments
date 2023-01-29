
add_library(MyIconLib INTERFACE)#  icon/My_Resource.rc)
target_include_directories(MyIconLib INTERFACE icon)

add_executable(AppWithResource_Exec code/TheMainForAppWithResource.cpp icon/My_Resource.rc)
target_link_libraries(AppWithResource_Exec PRIVATE MyIconLib)
