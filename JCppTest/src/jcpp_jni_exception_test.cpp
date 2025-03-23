#include "../catch2/catch_amalgamated.hpp"

#include <jni.h>

#include <jcpp_jni_exception.h>
#include <jcpp_manager.h>

TEST_CASE("Jni Exception 检查", "[jcpp_jni_exception]")
{
    jcpp::JCppManager::InitializeExplicitly();

	SECTION("> 无异常抛出")
	{
		jcpp::JCppManager::jvm_env_->ExceptionClear();

        REQUIRE_NOTHROW(jcpp::exception::CheckJniException());

        REQUIRE_NOTHROW(jcpp::exception::CheckJniExceptionIf(false));
        REQUIRE_NOTHROW(jcpp::exception::CheckJniExceptionIf(true));
	}

	SECTION("> 有异常抛出")
	{
		jcpp::JCppManager::jvm_env_->ThrowNew(
                jcpp::JCppManager::jvm_env_->FindClass("java/lang/Exception"), "test");

        REQUIRE_THROWS_AS(jcpp::exception::CheckJniException(), jcpp::exception::JniException);
	}

	SECTION("> 条件检查异常")
	{
		auto* jc_exception = jcpp::JCppManager::jvm_env_->FindClass("java/lang/Exception");

        if(jc_exception == nullptr)
        {
            REQUIRE_THROWS_AS(jcpp::exception::CheckJniExceptionIf(jc_exception == nullptr),
                    jcpp::exception::JniException);
        }
        else
        {
            REQUIRE_NOTHROW(jcpp::exception::CheckJniExceptionIf(jc_exception == nullptr));
        }
	}
}