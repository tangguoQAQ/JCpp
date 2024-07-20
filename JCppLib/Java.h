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
	 * @brief ��ȡ jclass �����档��Ҫ�ڶ��̻߳����е��ô˺�������Ϊ����ʹ�� unordered_map��
	 * @throws Java::Exception::JniException
	 */
	const std::shared_ptr<::jclass>* GetClassAndCache(ConstString className) noexcept(false);

}
