#pragma once

#include <string>
#include <functional>

using ConstString = const char*;

/**
 * @brief ʹ�� Java.h �е� 'LZSTR(str)' ������ֵ��
 */
using LazyString = std::function<std::string()>;
