#pragma once

#include <memory>
#include <utility>
#include <string>
#include <vector>

#include <jni.h>

#include "jcpp_util.h"

namespace jcpp
{

/**
 * @brief 指示 JNI 版本号。
 */
namespace jni_version
{
	using jni_version_t = int;

	constexpr jni_version_t JNI_1_1 = 0x00010001;
	constexpr jni_version_t JNI_1_2 = 0x00010002;
	constexpr jni_version_t JNI_1_4 = 0x00010004;
	constexpr jni_version_t JNI_1_6 = 0x00010006;
	constexpr jni_version_t JNI_1_8 = 0x00010008;
	constexpr jni_version_t JNI_9 = 0x00090000;
	constexpr jni_version_t JNI_10 = 0x000a0000;
	constexpr jni_version_t JNI_19 = 0x00130000;
	constexpr jni_version_t JNI_20 = 0x00140000;
	constexpr jni_version_t JNI_21 = 0x00150000;
}

struct JvmDeleter;
struct JvmEnvDeleter;

class JCppManager
{
public:
	static const std::vector<std::string> DEFAULT_JVM_OPTIONS;

	/**
	 * @brief 设置 JVM 的构造参数。
	 * @param version 请使用 `jcpp::jni_version::JNI_*`。
	 * @param options 缺省为 `{ "-Djava.class.path=.", "-Djava.compiler=NONE" }`
	 */
	static void SetConstructArgs(jni_version::jni_version_t version, const std::vector<std::string>& options = DEFAULT_JVM_OPTIONS);

	static inline jni_version::jni_version_t GetJniVersion() noexcept {
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
	static jni_version::jni_version_t jni_version_;
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
		util::ThrowIf(rc, "Failed to destory Java VM");
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