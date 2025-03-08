#include "jcpp_manager.h"

#include <utility>
#include <vector>
#include <string>

#include <jni_md.h>
#include <jni.h>
#include <memory>

#include "jcpp_util.h"

namespace jcpp
{

std::unique_ptr<::JavaVM, JvmDeleter> JCppManager::jvm_{};
std::unique_ptr<::JNIEnv, JvmEnvDeleter> JCppManager::jvm_env_{};

JCppManager::JniVersion JCppManager::jni_version_ = JniVersion::JNI_1_8;
const std::vector<std::string> JCppManager::DEFAULT_JVM_OPTIONS = { "-Djava.class.path=." };
std::vector<std::string> JCppManager::jvm_options_ = DEFAULT_JVM_OPTIONS;

static constexpr ::jint ToJniVersion(JCppManager::JniVersion version)
{
	switch(version)
	{
	case JCppManager::JniVersion::JNI_1_1:
		return 0x00010001;

	case JCppManager::JniVersion::JNI_1_2:
		return 0x00010002;

	case JCppManager::JniVersion::JNI_1_4:
		return 0x00010004;

	case JCppManager::JniVersion::JNI_1_6:
		return 0x00010006;

	case JCppManager::JniVersion::JNI_1_8:
		return 0x00010008;

	case JCppManager::JniVersion::JNI_9:
		return 0x00090000;

	case JCppManager::JniVersion::JNI_10:
		return 0x000a0000;

	case JCppManager::JniVersion::JNI_19:
		return 0x00130000;

	case JCppManager::JniVersion::JNI_20:
		return 0x00140000;

	case JCppManager::JniVersion::JNI_21:
		return 0x00150000;

	default:
		return 0x00010001;
	}
}

void JCppManager::SetConstructArgs(JniVersion version, const std::vector<std::string>& options)
{
	jni_version_ = version;
	jvm_options_ = options;
}

bool JCppManager::IsInitialized()
{
    return jvm_.get() != nullptr;
}

void JCppManager::InitializeJvmOnce()
{
	if(!jvm_) ConstructJvm();
}

void JCppManager::ConstructJvm()
{
	::JavaVM* jvm = nullptr;
	::JNIEnv* env = nullptr;

	const auto rc = JNI_CreateJavaVM(&jvm, reinterpret_cast<void**>(&env), GetConstructArgs().get());
	util::ThrowIf(rc != JNI_OK, "Failed to construct Java VM", rc);

	jvm_.reset(jvm);
	jvm_env_.reset(env);
}

std::unique_ptr<::JavaVMInitArgs> JCppManager::GetConstructArgs()
{

	std::unique_ptr<::JavaVMInitArgs> result = std::make_unique<::JavaVMInitArgs>();
	const ::size_t option_size = jvm_options_.size();
	result->nOptions = option_size;
	auto* jvm_option_arr = new ::JavaVMOption[option_size];
	for(int i = 0; i < option_size; i++)
	{
		jvm_option_arr[i].optionString = const_cast<char*>(jvm_options_[i].c_str());
	}
	result->options = jvm_option_arr;

	result->version = ToJniVersion(jni_version_);
	result->ignoreUnrecognized = false;

	return result;
}

void JCppManager::DestroyJvmOnce()
{
	if(!jvm_) return;

	jvm_.reset();	// may throw std::runtime_error
	jvm_env_.reset();
}

}