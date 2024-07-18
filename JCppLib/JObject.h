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
	private:
		char szClassName[MAX_CLASSPATH_LEN];
		::jclass selfClass = nullptr;

	protected:
		::jobject self = nullptr;

	public:
		/**
		 * @param pObject ������ɺ󽫱��ͷţ���ʹ�� Ptr() ������ȡ��ָ�롣
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

