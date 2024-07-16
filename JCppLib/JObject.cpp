#include "JObject.h"

namespace Java
{
	/**
	 * @param pObject 构造完成后将被释放，请使用 Ptr() 方法获取其指针。
	 */
	JObject::JObject(::jobject pObject)
	{
		Exception::ThrowIf(pObject == nullptr, { Exception::Jni, "pObject is null"});

		const auto pGlobalObject = static_cast<::jobject>(env->NewGlobalRef(pObject));
		env->DeleteLocalRef(pObject);
		ThrowIfChecked(Exception::Jni);
		self = pGlobalObject;
	}

	JObject::~JObject()
	{
		if(self != nullptr) env->DeleteGlobalRef(self);
	}

	bool operator==(const JObject& l, const JObject& r) noexcept
	{
		return env->IsSameObject(l.self, r.self);
	}

	bool operator!=(const JObject& l, const JObject& r) noexcept
	{
		return !(l == r);
	}

	std::ostream& operator<<(std::ostream& os, const JObject& obj)
	{
		os << "JObject@" << obj.Class() << ";" << obj.Do<::jstring>("toString", "()Ljava/lang/String;")wd;
		return os;
	}

	JClass JObject::Class() const noexcept(false)
	{
		::jclass pClass = env->GetObjectClass(self);
		::jobject pjoSelfClass = env->CallObjectMethod(self,
			env->GetMethodID(pClass, "getClass", "()Ljava/lang/Class;"));
		::jclass pjcClass = env->GetObjectClass(pjoSelfClass);
		::jstring jsClassName = static_cast<::jstring>(env->CallObjectMethod(pjoSelfClass,
			env->GetMethodID(pjcClass, "getName", "()Ljava/lang/String;")));
		char szClassName[MAX_CLASSPATH_LEN];
		auto len = env->GetStringLength(jsClassName);
		env->GetStringUTFRegion(jsClassName, 0, len, szClassName);

		env->DeleteLocalRef(pClass);
		env->DeleteLocalRef(pjoSelfClass);
		env->DeleteLocalRef(pjcClass);
		env->DeleteLocalRef(jsClassName);

		return JClass(szClassName);
	}

	::jobject JObject::Ptr() const noexcept
	{
		return self;
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

		const auto methodID = env->GetMethodID(Class().Ptr(), methodName, signature);
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
		else if constexpr(std::is_same_v<R, JObject>)
		{
			::jobject result = env->CallObjectMethodV(self, methodID, vaList);
			va_end(vaList);
			ThrowIfChecked(Exception::CallMethodFailed);
			return JObject(result);
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

			else if constexpr(std::is_same_v<R, ::jstring>)
				result = static_cast<::jstring>(env->CallObjectMethodV(self, methodID, vaList));

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
	template ::jstring JObject::Do(ConstString, ConstString, ...) const;
	
	JString::JString() noexcept(false) : JObject(env->NewStringUTF("")) {}

	JString::JString(ConstString str) noexcept(false) : JObject(env->NewStringUTF(str)) {}

}