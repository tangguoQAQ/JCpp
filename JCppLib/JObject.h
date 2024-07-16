#pragma once

#include "Java.h"

namespace Java
{
	class JClass;

	/**
	 * @brief jobject �ķ�װ�ࡣ������������Ϊ��jo�������������� joPool��
	 */
	class JObject
	{
	protected:
		::jobject self = nullptr;

	public:
		/**
		 * @param pObject ������ɺ󽫱��ͷţ���ʹ�� Ptr() ������ȡ��ָ�롣
		 */
		JObject(::jobject pObject);

		~JObject();

		friend bool operator==(const JObject& l, const JObject& r) noexcept;
		friend bool operator!=(const JObject& l, const JObject& r) noexcept;

		friend std::ostream& operator<<(std::ostream& os, const JObject& obj);

		::jobject Ptr() const noexcept;

		JClass Class() const noexcept(false);

		/**
		* @brief ���ö���ķ�����
		 * @tparam R �����ķ�������
		 * @param methodName ������
		 * @param signature ����ǩ������ "(Ljava/lang/String;)V"
		 * @param ... �����������ɿա���ʹ�ô���Ĳ�����JVM ���ܻ��׳� fatal error��
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

