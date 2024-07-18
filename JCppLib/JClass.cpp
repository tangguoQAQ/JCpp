#pragma once

#include "JClass.h"
#include "Util.h"
#include <cstdarg>
#include <stdexcept>
#include <memory>
#include <algorithm>
#include <type_traits>
#include <iostream>

namespace Java
{
	/**
	 * @param className 完整的类路径, 如 "java.lang.String"
	 * @throws Java::Exception::JniException
	 */
	JClass::JClass(ConstString className) noexcept(false) : name(className), self(*GetClassAndCache(className)) {}

	JClass::~JClass() noexcept
	{
		self.reset();
		GetClassAndCache((name + "?").c_str());
	}

	bool JClass::operator==(const JClass& another) const noexcept
	{
		return *self == *another.self;
	}

	bool JClass::operator!=(const JClass& another) const noexcept
	{
		return !(*this == another);
	}

	std::ostream& Java::operator<<(std::ostream& os, const JClass& jc)
	{
		return os << std::string("JClass@") + jc.name + ";";
	}

	::jclass JClass::Ptr() const noexcept
	{
		return *self;
	}

	/**
	 * @brief 构造该类的实例。
	 * @param signature 构造器方法签名
	 * @param ... 构造器参数，可空。若使用错误的参数，JVM 可能会抛出 fatal error。
	 * @throws Java::Exception::JniException
	 */
	JObject JClass::New(ConstString signature, ...) const noexcept(false)
	{
		const auto methodID = env->GetMethodID(*self, "<init>", signature);
		Exception::ThrowIf(methodID == nullptr, Exception::MethodNotFound,
			LZSTR((name + ".<init>" + signature).c_str()));

		va_list vaList;
		va_start(vaList, signature);
		::jobject pObject = env->NewObjectV(*self, methodID, vaList);
		va_end(vaList);
		Exception::ThrowIf(pObject == nullptr, Exception::NewObjectFailed);

		return JObject(pObject);
	}
}
