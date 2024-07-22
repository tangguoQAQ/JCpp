#pragma once

#include <exception>
#include <array>
#include <UsingAlias.h>
#include <functional>

namespace Util {
	void ThrowIf(bool b, char const* const expectionMessage);

	void ThrowIf(bool b, const std::exception& e);
}
