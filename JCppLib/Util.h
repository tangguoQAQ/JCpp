#pragma once

#include <exception>

namespace Util {
    void ThrowIf(bool b, char const* const expectionMessage);

    void ThrowIf(bool b, const std::exception& e);
}
