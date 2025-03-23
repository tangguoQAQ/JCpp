#include "jcpp_util.h"

#include <stdexcept>
#include <sstream>

#include <jni.h>

namespace jcpp::util
{

void ThrowIf(bool condition, char const* const expection_message)
{
    if(condition) throw std::runtime_error(expection_message);
}

void ThrowIf(int error_code, const char* expection_message)
{
    if (error_code == JNI_OK) return;

    std::ostringstream msg_stream{};
    msg_stream << expection_message << " (code: " << error_code << ")";
    throw std::runtime_error(msg_stream.str());
}

}
