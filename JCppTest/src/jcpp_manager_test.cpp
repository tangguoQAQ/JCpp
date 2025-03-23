#include "../catch2/catch_amalgamated.hpp"

#include <jcpp_manager.h>

TEST_CASE("JcppManager 初始化", "[jcpp_manager]")
{
	SECTION("> 设置构造参数")
	{
		jcpp::JCppManager::SetConstructArgs(jcpp::jni_version::JNI_10);

		REQUIRE(jcpp::JCppManager::GetJniVersion() == jcpp::jni_version::JNI_10);
		REQUIRE(jcpp::JCppManager::GetJvmOptions() == jcpp::JCppManager::DEFAULT_JVM_OPTIONS);
	}

	SECTION("> 显式初始化")
	{
		REQUIRE_NOTHROW(jcpp::JCppManager::InitializeExplicitly());

		REQUIRE(jcpp::JCppManager::GetJvmOptions() == jcpp::JCppManager::DEFAULT_JVM_OPTIONS);
		REQUIRE(jcpp::JCppManager::IsInitialized());
	}

	SECTION("> 过多初始化")
	{
		REQUIRE_NOTHROW(jcpp::JCppManager::InitializeExplicitly());
		
		REQUIRE(jcpp::JCppManager::IsInitialized());
	}
}

TEST_CASE("JcppManager 销毁", "[jcpp_manager]")
{
	SECTION("> 显式销毁")
	{
		REQUIRE_NOTHROW(jcpp::JCppManager::DestroyExplicitly());

		REQUIRE(!jcpp::JCppManager::IsInitialized());
	}

	SECTION("> 过多销毁")
	{
		REQUIRE_NOTHROW(jcpp::JCppManager::DestroyExplicitly());

		REQUIRE(!jcpp::JCppManager::IsInitialized());
	}
}
