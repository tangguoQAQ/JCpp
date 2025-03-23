#include "jcpp_jni_exception.h"

#include <type_traits>
#include <string>

#include <jni.h>

#include "jcpp_manager.h"

namespace jcpp::exception
{

const char* JniException::what() const noexcept
{
    return message_.what();
}

static_assert(std::is_nothrow_copy_constructible_v<JniException>, "JniException must be nothrow copy constructible");

namespace
{
    void ThrowJniException(::jthrowable jo_exception) noexcept(false)
    {
        auto* js_exception_msg = static_cast<::jstring>(JCppManager::jvm_env_->CallObjectMethod(jo_exception,
                JCppManager::jvm_env_->GetMethodID(
                        JCppManager::jvm_env_->GetObjectClass(jo_exception),
                        "toString", "()Ljava/lang/String;")));
        
		const char* sz_exception_msg = JCppManager::jvm_env_->GetStringUTFChars(js_exception_msg, nullptr);
        const JniException jni_exception{ std::string(sz_exception_msg) };

		JCppManager::jvm_env_->ReleaseStringUTFChars(js_exception_msg, sz_exception_msg);
        JCppManager::jvm_env_->DeleteLocalRef(jo_exception);
        JCppManager::jvm_env_->DeleteLocalRef(js_exception_msg);

        throw jni_exception;
    }
} // namespace

void CheckJniException() noexcept(false)
{
    auto* jo_exception = JCppManager::jvm_env_->ExceptionOccurred();
    if(jo_exception == nullptr)
    {
        return;
    }

    JCppManager::jvm_env_->ExceptionDescribe();
    JCppManager::jvm_env_->ExceptionClear();
    ThrowJniException(jo_exception);
}

void CheckJniExceptionIf(bool condition) noexcept(false)
{
    if(!condition)
    {
        return;
    }

    CheckJniException();
}

} // namespace jcpp::exception
