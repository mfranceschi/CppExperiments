//
// Created by Utilisateur on 12/03/2023.
//

#include "Utils.hpp"

wchar_t *ToWchar_t(const char *source) {
    size_t length = strlen(source);
    std::unique_ptr<wchar_t[]> destination =
            std::make_unique<wchar_t[]>(length + 1);
    size_t retValue = 0;

#if defined _MSC_VER || \
    (defined __STDC_LIB_EXT1__ && defined __STDC_WANT_LIB_EXT1__ && \
     __STDC_WANT_LIB_EXT1__ == 1)
    mbstowcs_s(&retValue, destination.get(), length + 1, source, length);
#else
    retValue = mbstowcs(destination.get(), source, length) - length;
#endif

    if (retValue == 0U) {
        destination.reset();
    }
    return destination.release();
}
