#include "catch2/catch_amalgamated.hpp"
#include <Java.h>
#include <iostream>

using namespace Java;

TEST_CASE("JObject", "[jobject][jclass]")
{
	const JClass jcInteger("java.lang.Integer");

	SECTION("> ʹ�� JClass.New() ���� JObject")
	{
		REQUIRE_NOTHROW(jcInteger.New("(I)V", ::jint(114514)));
	}

	SECTION("> ʹ�� JClass.New() ʱ�������Ĺ���������ǩ��")
	{
		REQUIRE_THROWS(jcInteger.New("bad signature", ::jint(114514)));
	}
}

TEST_CASE("JObject ����", "[jobject]")
{
	const JClass jcInteger("java.lang.Integer");
	auto joI = jcInteger.New("(I)V", ::jint(114514));

	SECTION("> ���÷���")
	{
		// TODO: ��������󽫰�װ���ʹ���
		auto d = joI.Do<::jdouble>("doubleValue", "()D");
		REQUIRE_THAT(d, Catch::Matchers::WithinRel(114514.0, 0.001));
	}

	SECTION("> ���þ�̬����")
	{
		std::cout << joI.Class() << std::endl;
		REQUIRE(joI.Class() == jcInteger);

		auto joI1 = joI.Class().Do<JObject>("valueOf", "(I)Ljava/lang/Integer;", ::jint(37));
		auto joI2 = jcInteger.Do<JObject>("valueOf", "(I)Ljava/lang/Integer;", ::jint(37));
		REQUIRE(joI1 == joI2);

		auto joI3 = joI.Class().Do<JObject>("valueOf", "(I)Ljava/lang/Integer;", ::jint(2147483647));
		auto joI4 = jcInteger.Do<JObject>("valueOf", "(I)Ljava/lang/Integer;", ::jint(2147483647));
		REQUIRE(joI3 != joI4);

		REQUIRE(joI.Class().Do<::jint>("parseInt", "(Ljava/lang/String;)I", JString("2147483647").Ptr()) == 2147483647);
	}
}