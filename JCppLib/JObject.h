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
	protected:
		::jobject self = nullptr;

	public:
		/**
		 * @param pObject 构造完成后将被释放，请使用 Ptr() 方法获取其指针。
		 */
		JObject(::jobject pObject);

		~JObject();

		friend bool operator==(const JObject& l, const JObject& r) noexcept;
		friend bool operator!=(const JObject& l, const JObject& r) noexcept;

		friend std::ostream& operator<<(std::ostream& os, const JObject& obj);

		::jobject Ptr() const noexcept;

		JClass Class() const noexcept(false);

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

	bool operator==(const JObject& l, const JObject& r) noexcept;
	bool operator!=(const JObject& l, const JObject& r) noexcept;

	std::ostream& operator<<(std::ostream& os, const JObject& obj);

	class JString : public JObject
	{
	public:
		using JObject::JObject;

		JString() noexcept(false);

		JString(ConstString str) noexcept(false);

		void Get(ConstString destination, size_t maxLen) const noexcept(false);

		operator std::string() const noexcept(false);
	};
}

