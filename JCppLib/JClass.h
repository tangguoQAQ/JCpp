#pragma once

#include "Java.h"
#include <unordered_map>
#include <memory>
#include "Util.h"

namespace Java
{
	class JObject;

	/**
	 * @brief jclass 的封装类，从此获取的一切生命周期由 JCpp 管理。变量建议命名为“jc【类名】”，如 jcClassPool。
	 */
	class JClass
	{
	protected:
		const std::string name;
		std::shared_ptr<::jclass> self = nullptr;

	public:
		/**
		 * @param className 完整的类路径, 如 "java.lang.String"
		 * @throws Java::Exception::JniException
		 */
		explicit JClass(ConstString className) noexcept(false);

		~JClass() noexcept;

		bool operator==(const JClass& another) const noexcept;
		bool operator!=(const JClass& another) const noexcept;

		friend std::ostream& operator<<(std::ostream& os, const JClass& jc);

		::jclass Ptr() const noexcept;

		ConstString Name() const noexcept;

#define GEN_SIGN_R(varName, args, R) std::ostringstream signStream;\
		signStream << "(";\
		((signStream << GetTypeCode(args)), ...);\
		signStream << ")";\
		signStream << GetTypeCode<R>();\
		const std::string varName = signStream.str();
#define GEN_SIGN_CR(varName, args, CR) std::ostringstream signStream;\
		signStream << "(";\
		((signStream << GetTypeCode(args)), ...);\
		signStream << ")";\
		signStream << GetTypeCode(CR);\
		const std::string varName = signStream.str();
#define NT(args) ToNative(args)...

		/**
		* @brief 调用类的静态方法。
		 * @tparam R 方法的返回类型
		 * @param methodName 方法名
		 * @param signature 方法签名，如 "(Ljava/lang/String;)V"
		 * @param ... 方法参数，可空。若使用错误的参数，JVM 可能会抛出 fatal error。
		 * @return R
		 * @throws Java::Exception::JniException
		 */
		template<typename R = void, typename... Args>
		R Do(ConstString methodName, const Args&... args) const noexcept(false)
		{
			GEN_SIGN_R(signature, &args, R);

			const auto methodID = env->GetStaticMethodID(*self, methodName, signature.c_str());
			Exception::ThrowIf(methodID == nullptr, Exception::MethodNotFound,
				LZSTR(name + "." + methodName + signature));

			if constexpr(std::is_void_v<R>)
			{
				env->CallStaticVoidMethod(*self, methodID, NT(args));
				ThrowIfChecked(Exception::CallMethodFailed);
				return;
			}
			else
			{
				R result;
				if constexpr(std::is_same_v<R, ::jboolean>)
					result = env->CallStaticBooleanMethod(*self, methodID, NT(args));

				else if constexpr(std::is_same_v<R, ::jchar>)
					result = env->CallStaticCharMethod(*self, methodID, NT(args));

				else if constexpr(std::is_same_v<R, ::jbyte>)
					result = env->CallStaticByteMethod(*self, methodID, NT(args));

				else if constexpr(std::is_same_v<R, ::jshort>)
					result = env->CallStaticShortMethod(*self, methodID, NT(args));

				else if constexpr(std::is_same_v<R, ::jint>)
					result = env->CallStaticIntMethod(*self, methodID, NT(args));

				else if constexpr(std::is_same_v<R, ::jlong>)
					result = env->CallStaticLongMethod(*self, methodID, NT(args));

				else if constexpr(std::is_same_v<R, ::jfloat>)
					result = env->CallStaticFloatMethod(*self, methodID, NT(args));

				else if constexpr(std::is_same_v<R, ::jdouble>)
					result = env->CallStaticDoubleMethod(*self, methodID, NT(args));

				else Util::ThrowIf(true, "Unsupported template argument R type.");

				ThrowIfChecked(Exception::CallMethodFailed);
				return result;
			}
		}

		template<typename R = JObject, typename... Args,
		typename = std::enable_if_t<std::is_same_v<R, JObject> || std::is_base_of_v<JObject, R>>>
		R Do(const JClass& returnType, ConstString methodName, const Args&... args) const noexcept(false)
		{
			GEN_SIGN_CR(signature, &args, &returnType);

			const auto methodID = env->GetStaticMethodID(*self, methodName, signature.c_str());
			Exception::ThrowIf(methodID == nullptr, Exception::MethodNotFound,
				LZSTR(name + "." + methodName + signature));

			::jobject result = env->CallObjectMethod(*self, methodID, NT(args));
			ThrowIfChecked(Exception::CallMethodFailed);
			return R(returnType, result);
		}
		
		/**
		 * @brief 构造该类的实例。
		 * @param signature 构造器方法签名
		 * @param ... 构造器参数，可空。若使用错误的参数，JVM 可能会抛出 fatal error。
		 * @throws Java::Exception::JniException
		 */
		template<typename... Args>
		JObject New(const Args&... args) const noexcept(false)
		{
			GEN_SIGN_R(signature, &args, void);

			const auto methodID = env->GetMethodID(*self, "<init>", signature.c_str());
			Exception::ThrowIf(methodID == nullptr, Exception::MethodNotFound,
				LZSTR(name + ".<init>" + signature));

			::jobject pObject = env->NewObject(*self, methodID, args...);
			Exception::ThrowIf(pObject == nullptr, Exception::NewObjectFailed);

			return JObject(pObject);
		}
	};

	std::ostream& operator<<(std::ostream& os, const JClass& jc);
}