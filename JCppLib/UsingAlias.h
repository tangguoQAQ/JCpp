#pragma once

#include <string>
#include <functional>

using ConstString = const char*;

/**
 * @brief 使用 Java.h 中的 'LZSTR(str)' 宏生成值。
 */
using LazyString = std::function<std::string()>;
