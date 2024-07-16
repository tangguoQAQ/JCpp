#pragma once

constexpr size_t MAX_CLASSPATH_LEN = 256;

#include <jni.h>
#include "UsingAlias.h"
#include "JClass.h"
#include <memory>
#include <unordered_map>
#include <EnumHelper.h>
#include "JObject.h"

namespace Java {
	extern JavaVM* jvm;
	/**
	 * @brief ͨ����Ӧ��ʹ�øö��󣬳������������ʲô��
	 */
	extern JNIEnv* env;

	/**
	 * @brief �������߳��е��ô˺�������Ҫ������������ Java::Destroy()��
	 * @throws Java::Exception::JniException
	 */
	void Initialize(ConstString costumClasspath = ".") noexcept(false);

	void Destroy() noexcept;

	namespace Exception
	{
		ENUM_DEFINE(EType,
			Jni,
			ClassNotFound,
			MethodNotFound,
			CallMethodFailed,
			NewObjectFailed,
			StringException
		);

		class JniException : public std::exception
		{
		private:
			EType m_t;

			constexpr static size_t MAX_MSG_LEN = 512;
			char szSelfMsg[MAX_MSG_LEN];

			friend void ThrowIfChecked(JniException e) noexcept(false);
			void SetMsg(::jstring msg);
		public:
			JniException(EType t, ConstString msg);
			JniException(ConstString msg);
			JniException(EType t);

			const char* what() const noexcept override;
		};

		/**
		 * @brief ÿ�ε����� JNI �ӿں󣬶�Ӧ�õ��ô˺���������Ƿ������쳣��
		 * @brief e.g. ThrowIfChecked(Exception::CallMethodFailed);
		 */
		void ThrowIfChecked(JniException e) noexcept(false);

		/**
		 * @brief �� b Ϊ�棬�������׳� JNI �Ƿ����쳣�������׳� e��
		 * @brief e.g. ThrowIf(pLocalClass == nullptr, JniException(ClassNotFound, className));
		 */
		void ThrowIf(bool b, JniException e) noexcept(false);
	}

	/**
	 * @brief ��ȡ jclass �����档��Ҫ�ڶ��̻߳����е��ô˺�������Ϊ����ʹ�� unordered_map��
	 * @throws Java::Exception::JniException
	 */
	const std::shared_ptr<::jclass>* GetClassAndCache(ConstString className) noexcept(false);

}
