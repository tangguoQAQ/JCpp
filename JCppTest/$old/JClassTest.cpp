#include "catch2/catch_amalgamated.hpp"
#include <Java.h>

TEST_CASE("JClass", "[jcalss]")
{
	SECTION("> 构造和析构 JClass")
	{
		REQUIRE_NOTHROW([&]() {
			Java::JClass jcSystem{ "java.lang.System" };
			Java::JClass jcSystemCopy = jcSystem;
			}());
	}

	SECTION("> 使用错误的类名构造 JClass 抛出异常")
		REQUIRE_THROWS([&]() {
		Java::JClass jcBad{ "bad.classpath.Bad" };
			}());
}


TEST_CASE("JClass 缓存", "[jcalss]")
{
	SECTION("> 使用缓存构造 Integer JClass 的指针相同")
	{
		Java::JClass jcInteger1{ "java.lang.Integer" };

		{
			Java::JClass jcInteger2{ "java.lang.Integer" };
			REQUIRE(jcInteger1.Ptr() == jcInteger2.Ptr());

			Java::JClass jcInteger3{ "java.lang.Integer" };
			REQUIRE(jcInteger1.Ptr() == jcInteger3.Ptr());
		}

		Java::JClass jcInteger4{ "java.lang.Integer" };
		REQUIRE(jcInteger1.Ptr() == jcInteger4.Ptr());
	}

	SECTION("> 缓存清除后可再次构造")
	{
		::jclass pClass = nullptr;
		{
			Java::JClass jcInteger1{ "java.lang.Integer" };
			pClass = jcInteger1.Ptr();
		}
		Java::JClass jcInteger2{ "java.lang.Integer" };
	}

	SECTION("> 不同类的 JClass 指针不同")
	{
		Java::JClass jcInteger{ "java.lang.Integer" };
		Java::JClass jcDouble{ "java.lang.Double" };

		REQUIRE(jcInteger.Ptr() != jcDouble.Ptr());
	}
}


TEST_CASE("JClass 静态方法", "[jclass]")
{
	const Java::JClass jcSystem{ "java.lang.System" };

	SECTION("> 调用无参方法")
	{
		auto t = jcSystem.Do<::jlong>("currentTimeMillis", "()J");
		INFO("System.currentTimeMillis(): " << t);
		REQUIRE(t != 0);
	}

	SECTION("> 调用有参方法")
	{
		auto jsAgr = Java::env->NewStringUTF("JAVA_HOME");
		auto jsJavaHome = jcSystem.Do<::jstring>("getenv", "(Ljava/lang/String;)Ljava/lang/String;", jsAgr);
		auto szJavaHome = Java::env->GetStringUTFChars(jsJavaHome, nullptr);
		INFO(R"(System.getenv("JAVA_HOME"): )" << szJavaHome);

		char* szJavaHomeInCpp;
		errno_t err = ::_dupenv_s(&szJavaHomeInCpp, nullptr, "JAVA_HOME");
		
		INFO(R"(::_dupenv_s(&szJavaHomeInCpp, nullptr, "JAVA_HOME"): )" << szJavaHomeInCpp);
		REQUIRE(::strncmp(szJavaHome, szJavaHomeInCpp, 512) == 0);

		if(!err) free(szJavaHomeInCpp);
		Java::env->ReleaseStringUTFChars(jsJavaHome, szJavaHome);
		Java::env->DeleteLocalRef(jsJavaHome);
		Java::env->DeleteLocalRef(jsAgr);
	}

	SECTION("> 调用无返回值方法")
	{
		REQUIRE_NOTHROW(jcSystem.Do("gc", "()V"));
	}

	SECTION("> 错误地调用方法")
	{
		SECTION(">> 不存在的方法名抛出异常")
			REQUIRE_THROWS(jcSystem.Do("badMethod", "()V"));

		SECTION(">> 不匹配的方法签名抛出异常")
			REQUIRE_THROWS(jcSystem.Do<::jint>("currentTimeMillis", "()I"));
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
	const Java::JClass jcSystem{ "java.lang.System" };
	auto joProperties = jcSystem.Do<Java::JObject>("getProperties", "()Ljava/util/Properties;");
	REQUIRE(joProperties.Ptr() != nullptr);

	int hashCode = joProperties.Do<::jint>("hashCode", "()I");
	INFO(joProperties);
	INFO("Properties.hashCode(): " << hashCode);
	REQUIRE(hashCode != 0);
}
