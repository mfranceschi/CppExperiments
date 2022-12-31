FetchContent_Declare(
        MFLib_Fetched
        GIT_REPOSITORY https://github.com/mfranceschi/MFranceschi_CppLibrary.git
        GIT_TAG master
)
FetchContent_MakeAvailable(MFLib_Fetched)
link_libraries(MF_ALL)
