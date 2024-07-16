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
	 * @brief 调用类的静态方法。
	 * @tparam R 方法的返回类型
	 * @param methodName 方法名
	 * @param signature 方法签名，如 "(Ljava/lang/String;)V"
	 * @param ... 方法参数，可空。若使用错误的参数，JVM 可能会抛出 fatal error。
	 * @return R
	 * @throws Java::Exception::JniException
	 */
	template<typename R>
	R JClass::Do(ConstString methodName, ConstString signature, ...) const noexcept(false)
	{
		using namespace Exception;

		const auto methodID = env->GetStaticMethodID(*self, methodName, signature);
		ThrowIf(methodID == nullptr, JniException(MethodNotFound, (name + "." + methodName + signature).c_str()));

		va_list vaList;
		va_start(vaList, signature);
		if constexpr(std::is_void_v<R>)
		{
			env->CallStaticVoidMethodV(*self, methodID, vaList);
			va_end(vaList);
			ThrowIfChecked(Exception::CallMethodFailed);
			return;
		}
		else if constexpr(std::is_same_v<R, JObject>)
		{
			::jobject result = env->CallStaticObjectMethodV(*self, methodID, vaList);
			va_end(vaList);
			ThrowIfChecked(Exception::CallMethodFailed);
			return JObject(result);
		}
		else
		{
			R result;
			if constexpr(std::is_same_v<R, ::jboolean>)
				result = env->CallStaticBooleanMethodV(*self, methodID, vaList);

			else if constexpr(std::is_same_v<R, ::jbyte>)
				result = env->CallStaticByteMethodV(*self, methodID, vaList);

			else if constexpr(std::is_same_v<R, ::jchar>)
				result = env->CallStaticCharMethodV(*self, methodID, vaList);
			
			else if constexpr(std::is_same_v<R, ::jshort>)
				result = env->CallStaticShortMethodV(*self, methodID, vaList);
			
			else if constexpr(std::is_same_v<R, ::jint>)
				result = env->CallStaticIntMethodV(*self, methodID, vaList);
			
			else if constexpr(std::is_same_v<R, ::jlong>)
				result = env->CallStaticLongMethodV(*self, methodID, vaList);
			
			else if constexpr(std::is_same_v<R, ::jfloat>)
				result = env->CallStaticFloatMethodV(*self, methodID, vaList);
			
			else if constexpr(std::is_same_v<R, ::jdouble>)
				result = env->CallStaticDoubleMethodV(*self, methodID, vaList);
			
			else if constexpr(std::is_same_v<R, ::jstring>)
				result = static_cast<::jstring>(env->CallStaticObjectMethodV(*self, methodID, vaList));
			
			else
				Util::ThrowIf(true, "Unsupported template argument R type.")
			
			va_end(vaList);
			ThrowIfChecked(Exception::CallMethodFailed);
			return result;
		}
	}

	template void JClass::Do(ConstString, ConstString, ...) const;
	template ::jboolean JClass::Do(ConstString, ConstString, ...) const;
	template ::jbyte JClass::Do(ConstString, ConstString, ...) const;
	template ::jchar JClass::Do(ConstString, ConstString, ...) const;
	template ::jshort JClass::Do(ConstString, ConstString, ...) const;
	template ::jint JClass::Do(ConstString, ConstString, ...) const;
	template ::jlong JClass::Do(ConstString, ConstString, ...) const;
	template ::jfloat JClass::Do(ConstString, ConstString, ...) const;
	template ::jdouble JClass::Do(ConstString, ConstString, ...) const;
	template JObject JClass::Do(ConstString, ConstString, ...) const;
	template ::jstring JClass::Do(ConstString, ConstString, ...) const;

	/**
	 * @brief 构造该类的实例。
	 * @param signature 构造器方法签名
	 * @param ... 构造器参数，可空。若使用错误的参数，JVM 可能会抛出 fatal error。
	 * @throws Java::Exception::JniException
	 */
	JObject JClass::New(ConstString signature, ...) const noexcept(false)
	{
		const auto methodID = env->GetMethodID(*self, "<init>", signature);
		using namespace Exception;
		ThrowIf(methodID == nullptr, JniException(MethodNotFound, (name + ".<init>" + signature).c_str()));

		va_list vaList;
		va_start(vaList, signature);
		::jobject pObject = env->NewObjectV(*self, methodID, vaList);
		va_end(vaList);
		ThrowIf(pObject == nullptr, Exception::NewObjectFailed);

		return pObject;
	}
}
