#include "catch2/catch_amalgamated.hpp"
#include <Java.h>

using namespace Java;

TEST_CASE("JClass", "[jcalss]")
{
	SECTION("> 构造和析构 JClass")
	{
		REQUIRE_NOTHROW([&]() {
			JClass jcSystem{ "java.lang.System" };
			JClass jcSystemCopy = jcSystem;
			}());
	}

	SECTION("> 使用错误的类名构造 JClass 抛出异常")
		REQUIRE_THROWS([&]() {
		JClass jcBad{ "bad.classpath.Bad" };
			}());
}


TEST_CASE("JClass 缓存", "[jcalss]")
{
	SECTION("> 使用缓存构造 Integer JClass 的指针相同")
	{
		JClass jcInteger1{ "java.lang.Integer" };

		{
			JClass jcInteger2{ "java.lang.Integer" };
			REQUIRE(jcInteger1.Ptr() == jcInteger2.Ptr());

			JClass jcInteger3{ "java.lang.Integer" };
			REQUIRE(jcInteger1.Ptr() == jcInteger3.Ptr());
		}

		JClass jcInteger4{ "java.lang.Integer" };
		REQUIRE(jcInteger1.Ptr() == jcInteger4.Ptr());
	}

	SECTION("> 缓存清除后可再次构造")
	{
		::jclass pClass = nullptr;
		{
			JClass jcInteger1{ "java.lang.Integer" };
			pClass = jcInteger1.Ptr();
		}
		JClass jcInteger2{ "java.lang.Integer" };
	}

	SECTION("> 不同类的 JClass 指针不同")
	{
		JClass jcInteger{ "java.lang.Integer" };
		JClass jcDouble{ "java.lang.Double" };

		REQUIRE(jcInteger.Ptr() != jcDouble.Ptr());
	}
}


TEST_CASE("JClass 静态方法", "[jclass]")
{
	const JClass jcSystem{ "java.lang.System" };

	SECTION("> 调用无参方法")
	{
		auto t = jcSystem.Do<::jlong>("currentTimeMillis");
		INFO("System.currentTimeMillis(): " << t);
		REQUIRE(t != 0);
	}

	SECTION("> 调用有参方法")
	{
		JClass jcString{ "java.lang.String" };
		JString joEnvName{ "JAVA_HOME" };
		auto jsJavaHome = jcSystem.Do<JString>(jcString, "getenv", joEnvName);
		char szJavaHome[512];
		jsJavaHome.Get(szJavaHome, 512);
		INFO(R"(System.getenv("JAVA_HOME"): )" << szJavaHome);

		char* szJavaHomeInCpp;
		errno_t err = ::_dupenv_s(&szJavaHomeInCpp, nullptr, "JAVA_HOME");
		
		INFO(R"(::_dupenv_s(&szJavaHomeInCpp, nullptr, "JAVA_HOME"): )" << szJavaHomeInCpp);
		REQUIRE(::strncmp(szJavaHome, szJavaHomeInCpp, 512) == 0);

		if(!err) free(szJavaHomeInCpp);
	}

	SECTION("> 调用无返回值方法")
	{
		REQUIRE_NOTHROW(jcSystem.Do("gc"));
	}

	SECTION("> 错误地调用方法")
	{
		SECTION(">> 不存在的方法名抛出异常")
			REQUIRE_THROWS(jcSystem.Do("badMethod"));

		SECTION(">> 不匹配的方法签名抛出异常")
			REQUIRE_THROWS(jcSystem.Do<::jint>("currentTimeMillis"));
	}

	SECTION("> 传递错误的参数")
	{
		INFO("全部抛出 fatal error");
	}

	SECTION("> 设置错误的返回值类型")
	{
		INFO("不会报错，但返回值未知");
	}
}

TEST_CASE("JClass JObject 方法", "[jclass][jobject]")
{
	JClass jcSystem{ "java.lang.System" };
	JClass jcProperties{ "java.util.Properties" };
	auto joProperties = jcSystem.Do(jcProperties, "getProperties");

	REQUIRE(joProperties.Ptr() != nullptr);

	int hashCode = joProperties.Do<::jint>("hashCode", "()I");
	INFO(joProperties);
	INFO("Properties.hashCode(): " << hashCode);
	REQUIRE(hashCode != 0);

	JClass jcInteger{ "java.lang.Integer" };
	JClass jcObject{ "java.lang.Object" };
	auto joInteger = jcInteger.New(::jint(123));
	auto iHashCode = jcSystem.Do<::jint>("identityHashCode", JCast(joInteger, jcObject));
	INFO("System.identityHashCode(): " << iHashCode);
	REQUIRE(iHashCode != 0);
}
