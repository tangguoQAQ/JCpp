#pragma once

namespace jcpp::util
{
void ThrowIf(bool condition, const char* expection_message);

void ThrowIf(int error_code, const char* expection_message);

}