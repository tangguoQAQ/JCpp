#include "jcpp_manager.h"

#include <utility>
#include <vector>
#include <string>
#include <mutex>

#include <jni_md.h>
#include <jni.h>
#include <memory>

#include "jcpp_util.h"

namespace jcpp
{

bool JCppManager::has_construct_args_ = false;
const std::vector<std::string> JCppManager::DEFAULT_JVM_OPTIONS = {
		"-Djava.class.path=.",
		"-Djava.compiler=NONE"};

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
	has_construct_args_ = true;

	jni_version_ = version;
	jvm_options_ = options;
}

void JCppManager::InitializeJvmOnce()
{
	std::call_once(jvm_init_flag_, [] { 
		ConstructJvm();
	});
}

void JCppManager::ConstructJvm()
{
	::JavaVM* jvm = nullptr;
	::JNIEnv* env = nullptr;

	const ::jint rc = JNI_CreateJavaVM(&jvm, reinterpret_cast<void**>(&env), GetConstructArgs().get());
	if (rc != JNI_OK)
	{
		throw std::runtime_error("Failed to create Java VM" + rc);
	}

	jvm_.reset(jvm);
	jvm_env_.reset(env);
}

std::unique_ptr<::JavaVMInitArgs> JCppManager::GetConstructArgs()
{
	if(!has_construct_args_)
	{
		SetConstructArgs(JniVersion::JNI_1_8, JCppManager::DEFAULT_JVM_OPTIONS);
	}

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

}