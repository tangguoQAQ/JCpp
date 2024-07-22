#pragma once

#include "UsingAlias.h"
#define LZSTR(str) [&]() -> std::string {return (str);}

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

			friend void ThrowIfChecked(const EType& t, std::string msg) noexcept(false);
			void SetMsg(::jstring msg);
		public:
			JniException(EType t, std::string msg);
			JniException(std::string msg);

			const char* what() const noexcept override;
		};

		void ThrowIfChecked(const EType& t = EType::Jni, std::string msg = "") noexcept(false);
		void ThrowIfChecked(const EType& t, const LazyString& msg) noexcept(false);

		void ThrowIf(bool b, const EType& t = EType::Jni, ConstString msg = "") noexcept(false);
		void ThrowIf(bool b, const EType& t, const LazyString& msg) noexcept(false);
		void ThrowIf(bool b, const LazyString& msg) noexcept(false);
		void ThrowIf(bool b, ConstString msg) noexcept(false);
	}

	/**
	 * @brief 获取 jclass 并缓存。不要在多线程环境中调用此函数，因为缓存使用 unordered_map。
	 * @throws Java::Exception::JniException
	 */
	const std::shared_ptr<::jclass>* GetClassAndCache(ConstString className) noexcept(false);

	template<typename T>
	std::string GetTypeCode(T* arg = nullptr)
	{
		using T_ = std::remove_cv_t<T>;
		if constexpr(std::is_same_v<T_, JClass> || std::is_base_of_v<JClass, T_> || std::is_same_v<T_, JObject> || std::is_base_of_v<JObject, T_>)
		{
			std::string name;
			if constexpr(std::is_same_v<T_, JClass> || std::is_base_of_v<JClass, T_>) name = arg->Name();
			else name = arg->ClassName();

			std::replace(name.begin(), name.end(), '.', '/');
			if(name.at(0) != '[')
			{
				name.insert(0, 1, 'L');
				name.append(";");
			}
			return name;
		}
		else if constexpr(std::is_same_v<T_, void>) return "V";
		else if constexpr(std::is_same_v<T_, ::jboolean>) return "Z";
		else if constexpr(std::is_same_v<T_, ::jbyte>) return "B";
		else if constexpr(std::is_same_v<T_, ::jchar>) return "C";
		else if constexpr(std::is_same_v<T_, ::jshort>) return "S";
		else if constexpr(std::is_same_v<T_, ::jint>) return "I";
		else if constexpr(std::is_same_v<T_, ::jlong>) return "J";
		else if constexpr(std::is_same_v<T_, ::jfloat>) return "F";
		else if constexpr(std::is_same_v<T_, ::jdouble>) return "D";
		// TODO: 支持数组类型。

		Util::ThrowIf(true, "Unsupported template argument T type.");
		return "";
	}

	template<typename T, typename R = std::conditional_t<std::is_same_v<T, JObject> || std::is_base_of_v<JObject, T>, ::jobject, T>>
	R ToNative(T arg)
	{
		using T_ = std::remove_cv_t<T>;
		if constexpr(std::is_same_v<T_, JObject> || std::is_base_of_v<JObject, T_>)
		{
			return arg.Ptr();
		}
		else return arg;
	}
}
