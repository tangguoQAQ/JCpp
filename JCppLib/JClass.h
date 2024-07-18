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
		 * @brief ��������ʵ����
		 * @param signature ����������ǩ��
		 * @param ... �������������ɿա���ʹ�ô���Ĳ�����JVM ���ܻ��׳� fatal error��
		 * @throws Java::Exception::JniException
		 */
		JObject New(ConstString constructorSignature, ...) const noexcept(false);
	};

	std::ostream& operator<<(std::ostream& os, const JClass& jc);
}