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
	static AtomicSingleton& getInstance() {
		std::call_once(initFlag, [] {
			instance.reset(new  AtomicSingleton());
			});
		return *instance;
	}

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

	static void SetConstructArgs(JniVersion version, const std::vector<std::string>& options);

	static inline JniVersion GetJniVersion() {
		return jni_version_;
	}

	static inline const std::vector<std::string>& GetJvmOptions() {
		return jvm_options_;
	}

private:
	static std::unique_ptr<JavaVM> jvm_;
	static std::once_flag jvm_init_flag_;
	static std::unique_ptr<JNIEnv> jvm_env_;

	static bool has_construct_args_;
	static JniVersion jni_version_;
	static std::vector<std::string> jvm_options_;

	/**
	 * @brief 仅第一次调用时初始化 JVM。
	 */
	static void InitializeJvmOnce();

	static std::pair<JavaVM*, JNIEnv*> ConstructJvm();

	static std::unique_ptr<::JavaVMInitArgs> GetConstructArgs();

};

}