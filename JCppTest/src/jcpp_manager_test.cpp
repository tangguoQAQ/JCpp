#include "../catch2/catch_amalgamated.hpp"
#include <jcpp_manager.h>

TEST_CASE("JcppManager 初始化", "[jcpp_manager]")
{
	SECTION("> 设置构造参数")
	{
		jcpp::JCppManager::SetConstructArgs(jcpp::JCppManager::JniVersion::JNI_10);

		REQUIRE(jcpp::JCppManager::GetJniVersion() == jcpp::JCppManager::JniVersion::JNI_10);
		REQUIRE(jcpp::JCppManager::GetJvmOptions() == jcpp::JCppManager::DEFAULT_JVM_OPTIONS);
	}

	SECTION("> 初始化")
	{
		jcpp::JCppManager::Initialize();

		REQUIRE(jcpp::JCppManager::GetJvmOptions() == jcpp::JCppManager::DEFAULT_JVM_OPTIONS);
	}
}
