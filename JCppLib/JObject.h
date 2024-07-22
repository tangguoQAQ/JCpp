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
		char szClassName[MAX_CLASSPATH_LEN] = { 0 };
		::jclass selfClass = nullptr;

		friend class JClass;
		JObject(const JClass& type, ::jobject pObject) noexcept(false);

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
