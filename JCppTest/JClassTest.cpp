#include "catch2/catch_amalgamated.hpp"
#include <Java.h>

TEST_CASE("JClass", "[jcalss]")
{
	SECTION("> ��������� JClass")
	{
		REQUIRE_NOTHROW([&]() {
			Java::JClass jcSystem{ "java.lang.System" };
			Java::JClass jcSystemCopy = jcSystem;
			}());
	}

	SECTION("> ʹ�ô������������ JClass �׳��쳣")
		REQUIRE_THROWS([&]() {
		Java::JClass jcBad{ "bad.classpath.Bad" };
			}());
}


TEST_CASE("JClass ����", "[jcalss]")
{
	SECTION("> ʹ�û��湹�� Integer JClass ��ָ����ͬ")
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

	SECTION("> �����������ٴι���")
	{
		::jclass pClass = nullptr;
		{
			Java::JClass jcInteger1{ "java.lang.Integer" };
			pClass = jcInteger1.Ptr();
		}
		Java::JClass jcInteger2{ "java.lang.Integer" };
	}

	SECTION("> ��ͬ��� JClass ָ�벻ͬ")
	{
		Java::JClass jcInteger{ "java.lang.Integer" };
		Java::JClass jcDouble{ "java.lang.Double" };

		REQUIRE(jcInteger.Ptr() != jcDouble.Ptr());
	}
}


TEST_CASE("JClass ��̬����", "[jclass]")
{
	const Java::JClass jcSystem{ "java.lang.System" };

	SECTION("> �����޲η���")
	{
		auto t = jcSystem.Do<::jlong>("currentTimeMillis", "()J");
		INFO("System.currentTimeMillis(): " << t);
		REQUIRE(t != 0);
	}

	SECTION("> �����вη���")
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

	SECTION("> �����޷���ֵ����")
	{
		REQUIRE_NOTHROW(jcSystem.Do("gc", "()V"));
	}

	SECTION("> ����ص��÷���")
	{
		SECTION(">> �����ڵķ������׳��쳣")
			REQUIRE_THROWS(jcSystem.Do("badMethod", "()V"));

		SECTION(">> ��ƥ��ķ���ǩ���׳��쳣")
			REQUIRE_THROWS(jcSystem.Do<::jint>("currentTimeMillis", "()I"));
	}

	SECTION("> ���ݴ���Ĳ���")
	{
		INFO("ȫ���׳� fatal error");
	}

	SECTION("> ���ô���ķ���ֵ����")
	{
		INFO("���ᱨ��������ֵδ֪");
	}
}

TEST_CASE("JClass JObject ����", "[jclass][jobject]")
{
	const Java::JClass jcSystem{ "java.lang.System" };
	auto joProperties = jcSystem.Do<Java::JObject>("getProperties", "()Ljava/util/Properties;");
	REQUIRE(joProperties.Ptr() != nullptr);

	int hashCode = joProperties.Do<::jint>("hashCode", "()I");
	INFO(joProperties);
	INFO("Properties.hashCode(): " << hashCode);
	REQUIRE(hashCode != 0);
}
