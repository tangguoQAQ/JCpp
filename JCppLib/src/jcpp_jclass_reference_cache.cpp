#include "jcpp_jclass_reference_cache.h"

#include <memory>
#include <string>

#include "jcpp_manager.h"
#include "jcpp_jni_exception.h"

namespace jcpp
{

JClassReferenceCache::Entry::Entry(const std::string& class_name) noexcept(false)
        : reference_(JCppManager::jvm_env_->FindClass(class_name.c_str()))
{
    exception::CheckJniExceptionIf(reference_ == nullptr);
}

std::shared_ptr<JClassReferenceCache::Entry> JClassReferenceCache::Get(const std::string& class_name) noexcept(false)
{
    auto iterator = cache_.find(class_name);

	if(iterator == cache_.end())
	{
		return cache_.emplace(class_name, std::make_shared<Entry>(class_name)).first->second;
	}

    return iterator->second;
}

void JClassReferenceCache::New(const std::string& class_name) noexcept(false)
{
    cache_.emplace(class_name, std::make_shared<Entry>(class_name));
}

}
