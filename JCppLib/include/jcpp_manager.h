#pragma once

#include <memory>
#include <mutex>
#include <utility>
#include <string>
#include <vector>

#include <jni.h>

namespace jcpp
{

class JCppManager
{
public:
	static const std::vector<std::string> DEFAULT_JVM_OPTIONS;

	enum class JniVersion
	{
		JNI_1_1,
		JNI_1_2,
		JNI_1_4,
		JNI_1_6,
		JNI_1_8,
		JNI_9,
		JNI_10,
		JNI_19,
		JNI_20,
		JNI_21
	};

	/**
	 * @brief 设置 JVM 的构造参数。
	 * @param options 缺省为 `{ "-Djava.class.path=.", "-Djava.compiler=NONE" }`
	 */
	static void SetConstructArgs(JniVersion version, const std::vector<std::string>& options = DEFAULT_JVM_OPTIONS);

	static inline JniVersion GetJniVersion() {
		return jni_version_;
	}

	static inline const std::vector<std::string>& GetJvmOptions() {
		return jvm_options_;
	}


	/**
	 * @brief 显式初始化 JCpp 库。
	 * 仅在首次调用或销毁后初始化一次，多次调用无效。
	 * @note 你一般不需要调用此函数，JCpp 会在合适的时候自动初始化，除非你希望显式控制 JVM 的初始化。
	 * @throw std::runtime_error JVM 构造失败。
	 */
	static void InitializeExplicitly()
	{
	    InitializeJvmOnce();
	}

private:
	static std::unique_ptr<::JavaVM, JvmDeleter> jvm_;
	static std::once_flag jvm_init_flag_;
	static std::unique_ptr<::JNIEnv> jvm_env_;

	static bool has_construct_args_;
	static JniVersion jni_version_;
	static std::vector<std::string> jvm_options_;

	/**
	 * @brief 仅第一次调用时初始化 JVM。
	 */
	static void InitializeJvmOnce();

	static void ConstructJvm();

	static std::unique_ptr<::JavaVMInitArgs> GetConstructArgs();

};

struct JvmDeleter
{
	void operator()(JavaVM* p) const
	{
		p->DestroyJavaVM();
	}
};

}