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
		char szClassName[MAX_CLASSPATH_LEN];
		::jclass selfClass = nullptr;

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

