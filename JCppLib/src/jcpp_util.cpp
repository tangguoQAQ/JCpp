#include "jcpp_util.h"

#include <stdexcept>

namespace jcpp
{
namespace util
{

void ThrowIf(bool b, char const* const expectionMessage)
{
    if(b) throw std::runtime_error(expectionMessage);
}

}

}
