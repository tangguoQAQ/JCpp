#include "JObject.h"
#include "Util.h"
#include <iostream>

namespace Java
{
	/**
	 * @param pObject 构造完成后将被释放，请使用 Ptr() 方法获取其指针。
	 */
	JObject::JObject(::jobject pObject) noexcept(false)
	{
		Exception::ThrowIf(pObject == nullptr, { Exception::Jni, "pObject is null"});

		const auto pGlobalObject = static_cast<::jobject>(env->NewGlobalRef(pObject));
		env->DeleteLocalRef(pObject);
		ThrowIfChecked(Exception::Jni);
		self = pGlobalObject;

		const auto pClass = env->GetObjectClass(self);
		selfClass = static_cast<::jclass>(env->NewGlobalRef(pClass));
		env->DeleteLocalRef(pClass);
		ThrowIfChecked(Exception::Jni);

		::jobject pjoSelfClass = env->CallObjectMethod(self,
			env->GetMethodID(selfClass, "getClass", "()Ljava/lang/Class;"));
		::jclass pjcClass = env->GetObjectClass(pjoSelfClass);
		::jstring jsClassName = static_cast<::jstring>(env->CallObjectMethod(pjoSelfClass,
			env->GetMethodID(pjcClass, "getName", "()Ljava/lang/String;")));
		env->GetStringUTFRegion(jsClassName, 0, env->GetStringLength(jsClassName), szClassName);
		env->DeleteLocalRef(pjoSelfClass);
		env->DeleteLocalRef(pjcClass);
		env->DeleteLocalRef(jsClassName);
		Exception::ThrowIfChecked(Exception::Jni);
	}

	JObject::~JObject() noexcept
	{
		if(self != nullptr) env->DeleteGlobalRef(self);
	}

	bool JObject::operator==(const JObject& another) const noexcept
	{
		return env->IsSameObject(self, another.self);
	}

	bool JObject::operator!=(const JObject& another) const noexcept
	{
		return !(*this == another);
	}

	std::ostream& operator<<(std::ostream& os, const JObject& jo)
	{
		return os << std::string("JObject@") + jo.szClassName + ";"
			+ jo.Do<JString>("toString", "()Ljava/lang/String;").Get() + ";";
	}

	::jobject JObject::Ptr() const noexcept
	{
		return self;
	}

	JClass JObject::Class() const noexcept(false)
	{
		return JClass(szClassName);
	}

	ConstString JObject::ClassName() const noexcept
	{
		return szClassName;
	}

	/**
	 * @brief 调用对象的方法。
	 * @tparam R 方法的返回类型
	 * @param methodName 方法名
	 * @param signature 方法签名，如 "(Ljava/lang/String;)V"
	 * @param ... 方法参数，可空。若使用错误的参数，JVM 可能会抛出 fatal error。
	 * @return R
	 * @throws Java::Exception::JniException
	 */
	template<typename R>
	R JObject::Do(ConstString methodName, ConstString signature, ...) const noexcept(false)
	{
		using namespace Java::Exception;

		const auto methodID = env->GetMethodID(selfClass, methodName, signature);
		ThrowIf(methodID == nullptr, JniException(MethodNotFound, (std::string(methodName) + signature).c_str()));

		va_list vaList;
		va_start(vaList, signature);
		if constexpr(std::is_void_v<R>)
		{
			env->CallVoidMethodV(self, methodID, vaList);
			va_end(vaList);
			ThrowIfChecked(Exception::CallMethodFailed);
			return;
		}
		else if constexpr(std::is_same_v<R, JObject> || std::is_base_of_v<JObject, R>)
		{
			::jobject result = env->CallObjectMethodV(self, methodID, vaList);
			va_end(vaList);
			ThrowIfChecked(Exception::CallMethodFailed);
			return R(result);
		}
		else
		{
			R result;
			if constexpr(std::is_same_v<R, ::jboolean>)
				result = env->CallBooleanMethodV(self, methodID, vaList);

			else if constexpr(std::is_same_v<R, ::jbyte>)
				result = env->CallByteMethodV(self, methodID, vaList);

			else if constexpr(std::is_same_v<R, ::jchar>)
				result = env->CallCharMethodV(self, methodID, vaList);

			else if constexpr(std::is_same_v<R, ::jshort>)
				result = env->CallShortMethodV(self, methodID, vaList);

			else if constexpr(std::is_same_v<R, ::jint>)
				result = env->CallIntMethodV(self, methodID, vaList);

			else if constexpr(std::is_same_v<R, ::jlong>)
				result = env->CallLongMethodV(self, methodID, vaList);

			else if constexpr(std::is_same_v<R, ::jfloat>)
				result = env->CallFloatMethodV(self, methodID, vaList);

			else if constexpr(std::is_same_v<R, ::jdouble>)
				result = env->CallDoubleMethodV(self, methodID, vaList);

			else
				Util::ThrowIf(true, "Unsupported template argument R type.")

				va_end(vaList);
			ThrowIfChecked(Exception::CallMethodFailed);
			return result;
		}
	}

	template void JObject::Do(ConstString, ConstString, ...) const;
	template ::jboolean JObject::Do(ConstString, ConstString, ...) const;
	template ::jbyte JObject::Do(ConstString, ConstString, ...) const;
	template ::jchar JObject::Do(ConstString, ConstString, ...) const;
	template ::jshort JObject::Do(ConstString, ConstString, ...) const;
	template ::jint JObject::Do(ConstString, ConstString, ...) const;
	template ::jlong JObject::Do(ConstString, ConstString, ...) const;
	template ::jfloat JObject::Do(ConstString, ConstString, ...) const;
	template ::jdouble JObject::Do(ConstString, ConstString, ...) const;
	template JObject JObject::Do(ConstString, ConstString, ...) const;
	

	JString::JString() noexcept(false) : JObject(env->NewStringUTF("")) {}

	JString::JString(ConstString str) noexcept(false) : JObject(env->NewStringUTF(str)) {}

	JString::operator std::string() const noexcept(false)
	{
		return Get();
	}

	void JString::Get(char* destination, size_t maxLen) const noexcept(false)
	{
		Util::ThrowIf(destination == nullptr, "destination is null");

		auto len = env->GetStringUTFLength(static_cast<::jstring>(self));
		if(len > maxLen) len = static_cast<::jsize>(maxLen);
		env->GetStringUTFRegion(static_cast<::jstring>(self), 0, len, destination);

		Exception::ThrowIfChecked(Exception::StringException);
	}

	std::string JString::Get() const noexcept(false)
	{
		const char* szStr = env->GetStringUTFChars(static_cast<::jstring>(self), nullptr);
		Exception::ThrowIf(szStr == nullptr, { Exception::StringException , "GetStringUTFChars failed"});

		std::string str(szStr);
		env->ReleaseStringUTFChars(static_cast<::jstring>(self), szStr);
		return str;
	}
}