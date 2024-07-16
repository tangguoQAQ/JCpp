#include "catch.hpp"
#include <Java.h>

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
		auto cmp = joI.Do<::jint>("compareTo", "(Ljava/lang/Integer;)I", jcInteger.New("(I)V", ::jint(2048)));
		REQUIRE(cmp > 0);
	}

	SECTION("> ���þ�̬����")
	{
		REQUIRE(joI.Class() == jcInteger);

		auto joI1 = joI.Class().Do<JObject>("valueOf", "(I)Ljava/lang/Integer;", ::jint(37));
		auto joI2 = jcInteger.Do<JObject>("valueOf", "(I)Ljava/lang/Integer;", ::jint(37));

		REQUIRE(joI1 == joI2);
	}
}