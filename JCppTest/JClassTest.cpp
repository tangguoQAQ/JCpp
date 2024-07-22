#include "catch2/catch_amalgamated.hpp"
#include <Java.h>

using namespace Java;

TEST_CASE("JClass", "[jcalss]")
{
	SECTION("> ��������� JClass")
	{
		REQUIRE_NOTHROW([&]() {
			JClass jcSystem{ "java.lang.System" };
			JClass jcSystemCopy = jcSystem;
			}());
	}

	SECTION("> ʹ�ô������������ JClass �׳��쳣")
		REQUIRE_THROWS([&]() {
		JClass jcBad{ "bad.classpath.Bad" };
			}());
}


TEST_CASE("JClass ����", "[jcalss]")
{
	SECTION("> ʹ�û��湹�� Integer JClass ��ָ����ͬ")
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

	SECTION("> �����������ٴι���")
	{
		::jclass pClass = nullptr;
		{
			JClass jcInteger1{ "java.lang.Integer" };
			pClass = jcInteger1.Ptr();
		}
		JClass jcInteger2{ "java.lang.Integer" };
	}

	SECTION("> ��ͬ��� JClass ָ�벻ͬ")
	{
		JClass jcInteger{ "java.lang.Integer" };
		JClass jcDouble{ "java.lang.Double" };

		REQUIRE(jcInteger.Ptr() != jcDouble.Ptr());
	}
}


TEST_CASE("JClass ��̬����", "[jclass]")
{
	const JClass jcSystem{ "java.lang.System" };

	SECTION("> �����޲η���")
	{
		auto t = jcSystem.Do<::jlong>("currentTimeMillis");
		INFO("System.currentTimeMillis(): " << t);
		REQUIRE(t != 0);
	}

	SECTION("> �����вη���")
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

	SECTION("> �����޷���ֵ����")
	{
		REQUIRE_NOTHROW(jcSystem.Do("gc"));
	}

	SECTION("> ����ص��÷���")
	{
		SECTION(">> �����ڵķ������׳��쳣")
			REQUIRE_THROWS(jcSystem.Do("badMethod"));

		SECTION(">> ��ƥ��ķ���ǩ���׳��쳣")
			REQUIRE_THROWS(jcSystem.Do<::jint>("currentTimeMillis"));
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
