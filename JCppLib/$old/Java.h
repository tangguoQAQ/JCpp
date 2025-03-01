#pragma once

#include "UsingAlias.h"
#define LZSTR(str) [&]() -> ConstString {return str;}

constexpr size_t MAX_CLASSPATH_LEN = 256;

#include <jni.h>
#include "JClass.h"
#include <memory>
#include <unordered_map>
#include <EnumHelper.h>
#include "JObject.h"
#include <functional>

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

			friend void ThrowIfChecked(const EType& t, ConstString msg) noexcept(false);
			void SetMsg(::jstring msg);
		public:
			JniException(EType t, ConstString msg);
			JniException(ConstString msg);
			JniException(EType t);

			const char* what() const noexcept override;
		};

		void ThrowIfChecked(const EType& t = Jni, ConstString msg = "") noexcept(false);
		void ThrowIfChecked(const EType& t, const LazyString& msg) noexcept(false);

		void ThrowIf(bool b, const EType& t = Jni, ConstString msg = "") noexcept(false);
		void ThrowIf(bool b, const EType& t, const LazyString& msg) noexcept(false);
		void ThrowIf(bool b, const LazyString& msg) noexcept(false);
		void ThrowIf(bool b, ConstString msg) noexcept(false);
	}

	/**
	 * @brief 获取 jclass 并缓存。不要在多线程环境中调用此函数，因为缓存使用 unordered_map。
	 * @throws Java::Exception::JniException
	 */
	const std::shared_ptr<::jclass>* GetClassAndCache(ConstString className) noexcept(false);

}
