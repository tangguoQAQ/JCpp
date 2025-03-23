#pragma once

#include <stdexcept>
#include <string>

namespace jcpp::exception
{

class JniException : public std::exception
{
public:
    JniException(const std::string& message) : message_(message) {}

    const char* what() const noexcept override;

private:
    const std::runtime_error message_;
};

void CheckJniException() noexcept(false);

void CheckJniExceptionIf(bool condition) noexcept(false);

}