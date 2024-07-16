#pragma once

#include "Java.h"
#include <unordered_map>
#include <memory>

namespace Java
{
	class JObject;

	/**
	 * @brief jclass �ķ�װ�࣬�Ӵ˻�ȡ��һ������������ JCpp ����������������Ϊ��jc�������������� jcClassPool��
	 */
	class JClass
	{
	protected:
		const std::string name;
		std::shared_ptr<::jclass> self = nullptr;

	public:

		/**
		 * @param className ��������·��, �� "java.lang.String"
		 * @throws Java::Exception::JniException
		 */
		explicit JClass(ConstString className) noexcept(false);

		~JClass() noexcept;

		bool operator==(const JClass& another) const noexcept;
		bool operator!=(const JClass& another) const noexcept;

		friend std::ostream& operator<<(std::ostream& os, const JClass& jc);

		::jclass Ptr() const noexcept;

		/**
		* @brief ������ľ�̬������
		 * @tparam R �����ķ�������
		 * @param methodName ������
		 * @param signature ����ǩ������ "(Ljava/lang/String;)V"
		 * @param ... �����������ɿա���ʹ�ô���Ĳ�����JVM ���ܻ��׳� fatal error��
		 * @return R
		 * @throws Java::Exception::JniException
		 */
		template<typename R = void>
		R Do(ConstString methodName, ConstString signature, ...) const noexcept(false);
		
		/**
		 * @brief ��������ʵ����
		 * @param signature ����������ǩ��
		 * @param ... �������������ɿա���ʹ�ô���Ĳ�����JVM ���ܻ��׳� fatal error��
		 * @throws Java::Exception::JniException
		 */
		JObject New(ConstString constructorSignature, ...) const noexcept(false);
	};

	std::ostream& operator<<(std::ostream& os, const JClass& jc);
}