#pragma once

#include "Java.h"

namespace Java
{
	class JClass;

	/**
	 * @brief jobject 的封装类。变量建议命名为“jo【类名】”，如 joPool。
	 */
	class JObject
	{
	private:
		char szClassName[MAX_CLASSPATH_LEN] = { 0 };
		::jclass selfClass = nullptr;

		friend class JClass;
		JObject(const JClass& type, ::jobject pObject) noexcept(false);

	protected:
		::jobject self = nullptr;

	public:
		/**
		 * @param pObject 构造完成后将被释放，请使用 Ptr() 方法获取其指针。
		 */
		explicit JObject(::jobject pObject) noexcept(false);

		~JObject() noexcept;

		bool operator==(const JObject& another) const noexcept;
		bool operator!=(const JObject& another) const noexcept;

		JObject operator=(const JObject& another) = delete;

		friend std::ostream& operator<<(std::ostream& os, const JObject& jo);

		::jobject Ptr() const noexcept;

		JClass Class() const noexcept(false);

		ConstString ClassName() const noexcept;

		template<typename R = JObject>
		R&& Cast(const JClass& targetClass) noexcept(false)
		{
			if(self != nullptr) env->DeleteGlobalRef(self);
			const auto pClass = targetClass.Ptr();
			selfClass = static_cast<::jclass>(env->NewGlobalRef(pClass));
			env->DeleteLocalRef(pClass);
			ThrowIfChecked(Exception::Jni);
			::strncpy(szClassName, targetClass.Name(), MAX_CLASSPATH_LEN);
			return static_cast<R&&>(*this);
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
		template<typename R = void>
		R Do(ConstString methodName, ConstString signature, ...) const noexcept(false);
	};

	template<typename R = JObject, typename T>
	R&& JCast(T&& origin, const JClass& targetClass) noexcept(false)
	{
		return origin.Cast<R>(targetClass);
	}

	class JString : public JObject
	{
	public:
		using JObject::JObject;

		JString() noexcept(false);

		explicit JString(ConstString str) noexcept(false);

		operator std::string() const noexcept(false);

		void Get(char* destination, size_t maxLen) const noexcept(false);

		std::string Get() const noexcept(false);
	};

	std::ostream& operator<<(std::ostream& os, const JObject& jo);
}
