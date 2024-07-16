#include "catch.hpp"
#include <Java.h>

using namespace Java;

TEST_CASE("JObject", "[jobject][jclass]")
{
	const JClass jcInteger("java.lang.Integer");

	SECTION("> 使用 JClass.New() 创建 JObject")
	{
		REQUIRE_NOTHROW(jcInteger.New("(I)V", ::jint(114514)));
	}

	SECTION("> 使用 JClass.New() 时传入错误的构造器方法签名")
	{
		REQUIRE_THROWS(jcInteger.New("bad signature", ::jint(114514)));
	}
}

TEST_CASE("JObject 方法", "[jobject]")
{
	const JClass jcInteger("java.lang.Integer");
	auto joI = jcInteger.New("(I)V", ::jint(114514));

	SECTION("> 调用方法")
	{
		auto cmp = joI.Do<::jint>("compareTo", "(Ljava/lang/Integer;)I", jcInteger.New("(I)V", ::jint(2048)));
		REQUIRE(cmp > 0);
	}

	SECTION("> 调用静态方法")
	{
		REQUIRE(joI.Class() == jcInteger);

		auto joI1 = joI.Class().Do<JObject>("valueOf", "(I)Ljava/lang/Integer;", ::jint(37));
		auto joI2 = jcInteger.Do<JObject>("valueOf", "(I)Ljava/lang/Integer;", ::jint(37));

		REQUIRE(joI1 == joI2);
	}
}