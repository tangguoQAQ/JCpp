#pragma once

#include "Java.h"
#include <unordered_map>
#include <memory>

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

		/**
		* @brief 调用类的静态方法。
		 * @tparam R 方法的返回类型
		 * @param methodName 方法名
		 * @param signature 方法签名，如 "(Ljava/lang/String;)V"
		 * @param ... 方法参数，可空。若使用错误的参数，JVM 可能会抛出 fatal error。
		 * @return R
		 * @throws Java::Exception::JniException
		 */
		template<typename R = void>
		R Do(ConstString methodName, ConstString signature, ...) const noexcept(false)
		{
			const auto methodID = env->GetStaticMethodID(*self, methodName, signature);
			Exception::ThrowIf(methodID == nullptr, Exception::MethodNotFound,
				LZSTR((name + "." + methodName + signature).c_str()));

			va_list vaList;
			va_start(vaList, signature);
			if constexpr(std::is_void_v<R>)
			{
				env->CallStaticVoidMethodV(*self, methodID, vaList);
				va_end(vaList);
				ThrowIfChecked(Exception::CallMethodFailed);
				return;
			}
			else if constexpr(std::is_same_v<R, JObject>)
			{
				::jobject result = env->CallStaticObjectMethodV(*self, methodID, vaList);
				va_end(vaList);
				ThrowIfChecked(Exception::CallMethodFailed);
				return JObject(result);
			}
			else
			{
				R result;
				if constexpr(std::is_same_v<R, ::jboolean>)
					result = env->CallStaticBooleanMethodV(*self, methodID, vaList);

				else if constexpr(std::is_same_v<R, ::jchar>)
					result = env->CallStaticCharMethodV(*self, methodID, vaList);

				else if constexpr(std::is_same_v<R, ::jbyte>)
					result = env->CallStaticByteMethodV(*self, methodID, vaList);

				else if constexpr(std::is_same_v<R, ::jshort>)
					result = env->CallStaticShortMethodV(*self, methodID, vaList);

				else if constexpr(std::is_same_v<R, ::jint>)
					result = env->CallStaticIntMethodV(*self, methodID, vaList);

				else if constexpr(std::is_same_v<R, ::jlong>)
					result = env->CallStaticLongMethodV(*self, methodID, vaList);

				else if constexpr(std::is_same_v<R, ::jfloat>)
					result = env->CallStaticFloatMethodV(*self, methodID, vaList);

				else if constexpr(std::is_same_v<R, ::jdouble>)
					result = env->CallStaticDoubleMethodV(*self, methodID, vaList);

				else if constexpr(std::is_same_v<R, ::jstring>)
					result = static_cast<::jstring>(env->CallStaticObjectMethodV(*self, methodID, vaList));

				else
					Util::ThrowIf(true, "Unsupported template argument R type.")

					va_end(vaList);
				ThrowIfChecked(Exception::CallMethodFailed);
				return result;
			}
		}

		
		/**
		 * @brief 构造该类的实例。
		 * @param signature 构造器方法签名
		 * @param ... 构造器参数，可空。若使用错误的参数，JVM 可能会抛出 fatal error。
		 * @throws Java::Exception::JniException
		 */
		JObject New(ConstString constructorSignature, ...) const noexcept(false);
	};

	std::ostream& operator<<(std::ostream& os, const JClass& jc);
}