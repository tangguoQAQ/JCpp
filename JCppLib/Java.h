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
	 * @brief 通常不应该使用该对象，除非你清楚在做什么。
	 */
	extern JNIEnv* env;

	/**
	 * @brief 请在主线程中调用此函数。不要忘记在最后调用 Java::Destroy()。
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
		 * @brief 每次调用完 JNI 接口后，都应该调用此函数来检查是否发生了异常。
		 * @brief e.g. ThrowIfChecked(Exception::CallMethodFailed);
		 */
		void ThrowIfChecked(JniException e) noexcept(false);

		/**
		 * @brief 若 b 为真，则优先抛出 JNI 是否发生异常，若无抛出 e。
		 * @brief e.g. ThrowIf(pLocalClass == nullptr, JniException(ClassNotFound, className));
		 */
		void ThrowIf(bool b, JniException e) noexcept(false);
	}

	/**
	 * @brief 获取 jclass 并缓存。不要在多线程环境中调用此函数，因为缓存使用 unordered_map。
	 * @throws Java::Exception::JniException
	 */
	const std::shared_ptr<::jclass>* GetClassAndCache(ConstString className) noexcept(false);

}
