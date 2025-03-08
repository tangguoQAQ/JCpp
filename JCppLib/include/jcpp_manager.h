#pragma once

#include <memory>
#include <utility>
#include <string>
#include <vector>

#include <jni.h>

#include "jcpp_util.h"

namespace jcpp
{

struct JvmDeleter;
struct JvmEnvDeleter;

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

	static inline JniVersion GetJniVersion() noexcept {
		return jni_version_;
	}

	static inline const std::vector<std::string>& GetJvmOptions() noexcept {
		return jvm_options_;
	}

	/**
	 * @brief 显式初始化 JCpp 库。
	 * 请在主线程调用。仅在首次调用或销毁后初始化一次，多次调用无效。
	 * @note 你一般不需要调用此函数，JCpp 会在合适的时候自动初始化，除非你希望显式控制 JVM 的初始化。
	 * @throw std::runtime_error JVM 构造失败。
	 */
	static inline void InitializeExplicitly() noexcept(false)
	{
	    InitializeJvmOnce();
	}

	static bool IsInitialized();

	/**
	 * @brief 显式销毁 JCpp 库。
	 * 请在主线程调用。仅在第一次调用或显式初始化后销毁一次，多次调用无效。
	 * @note 你一般不需要调用此函数，JCpp 会在合适的时候自动销毁，除非你希望显式控制 JVM 的销毁。
	 * @throw std::runtime_error JVM 销毁失败。
	 */
	static inline void DestroyExplicitly() noexcept(false)
	{
		DestroyJvmOnce();
	}

private:
	static std::unique_ptr<::JavaVM, JvmDeleter> jvm_;
	static std::unique_ptr<::JNIEnv, JvmEnvDeleter> jvm_env_;

	// Construct args
	static JniVersion jni_version_;
	static std::vector<std::string> jvm_options_;

	/**
	 * @brief 仅第一次调用时初始化 JVM。
	 */
	static void InitializeJvmOnce();

	static void ConstructJvm();

	static std::unique_ptr<::JavaVMInitArgs> GetConstructArgs();

	static void DestroyJvmOnce();

};

struct JvmDeleter
{
	void operator()(::JavaVM* p) const
	{
		const auto rc = p->DestroyJavaVM();
		util::ThrowIf(rc != JNI_OK, "Failed to destory Java VM", rc);
	}
};

struct JvmEnvDeleter
{
	void operator()(::JNIEnv* p) const
	{
		return;	// do nothing
	}
};

}