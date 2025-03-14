#include "jcpp_manager.h"

#include <cstddef>
#include <string>
#include <vector>
#include <memory>

#include <jni.h>
#include <jni_md.h>
#include <gsl/gsl>

#include "jcpp_util.h"

namespace jcpp
{

std::unique_ptr<::JavaVM, JvmDeleter> JCppManager::jvm_{};
std::unique_ptr<::JNIEnv, JvmEnvDeleter> JCppManager::jvm_env_{};

int JCppManager::jni_version_ = jni_version::JNI_1_8;
const std::vector<std::string> JCppManager::DEFAULT_JVM_OPTIONS{ "-Djava.class.path=." };
std::vector<std::string> JCppManager::jvm_options_ = DEFAULT_JVM_OPTIONS;

void JCppManager::SetConstructArgs(jni_version::jni_version_t version, const std::vector<std::string>& options)
{
	jni_version_ = version;
	jvm_options_ = options;
}

bool JCppManager::IsInitialized()
{
    return jvm_ != nullptr;
}

void JCppManager::InitializeJvmOnce()
{
    if(!jvm_)
    {
        ConstructJvm();
    }
}

void JCppManager::ConstructJvm()
{
	::JavaVM* jvm = nullptr;
	::JNIEnv* env = nullptr;

	const auto res = JNI_CreateJavaVM(&jvm, reinterpret_cast<void**>(&env), GetConstructArgs().get());
	util::ThrowIf(res, "Failed to construct Java VM");

	jvm_.reset(jvm);
	jvm_env_.reset(env);
}

std::unique_ptr<::JavaVMInitArgs> JCppManager::GetConstructArgs()
{

	std::unique_ptr<::JavaVMInitArgs> result = std::make_unique<::JavaVMInitArgs>();
	const ::size_t option_size = jvm_options_.size();
	result->nOptions = static_cast<jint>(option_size);
	gsl::owner<::JavaVMOption*> jvm_option_arr = new ::JavaVMOption[option_size];
#pragma unroll 8
	for(int i = 0; i < jvm_options_.size(); i++)
	{
		jvm_option_arr[i].optionString = const_cast<char*>(jvm_options_[i].c_str());
	}
	result->options = jvm_option_arr;

	result->version = jni_version_;
	result->ignoreUnrecognized = JNI_FALSE;

	return result;
}

void JCppManager::DestroyJvmOnce()
{
    if(!jvm_)
    {
        return;
    }

	jvm_.reset();	// may throw std::runtime_error
	jvm_env_.reset();
}

} // namespace jcpp