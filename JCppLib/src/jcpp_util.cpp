#include "jcpp_util.h"

#include <stdexcept>
#include <sstream>

namespace jcpp
{
namespace util
{

void ThrowIf(bool condition, char const* const expection_message)
{
    if(condition) throw std::runtime_error(expection_message);
}

void ThrowIf(bool condition, const char* expection_message, int error_code)
{
    if (!condition) return;

    std::ostringstream msg_stream{};
    msg_stream << expection_message << " (code: " << error_code << ")";
    throw std::runtime_error(msg_stream.str());
}

}
}
