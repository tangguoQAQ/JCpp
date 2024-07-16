#include "catch2/catch_amalgamated.hpp"
#include <Java.h>
#include <iostream>

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
		// TODO: 传入参数后将包装类型处理。
		auto d = joI.Do<::jdouble>("doubleValue", "()D");
		REQUIRE_THAT(d, Catch::Matchers::WithinRel(114514.0, 0.001));
	}

	SECTION("> 调用静态方法")
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