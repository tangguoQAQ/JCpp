#pragma once

namespace jcpp
{
namespace util
{
void ThrowIf(bool condition, const char* expection_message);

void ThrowIf(bool condition, const char* expection_message, int error_code);
}
}