#include <exception>
#include <iostream>
#include <stdexcept>

namespace Util
{
	void ThrowIf(bool b, char const* const expectionMessage)
	{
		if(b) throw std::runtime_error(expectionMessage);
	}

	void ThrowIf(bool b, const std::exception& e)
	{
		if(b) throw e;
	}
}