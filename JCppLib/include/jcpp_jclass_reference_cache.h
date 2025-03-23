#pragma once

#include <string>
#include <unordered_map>
#include <memory>

#include <jni.h>

namespace jcpp
{
class JClassReferenceCache
{
public:

    class Entry
    {
    public:
        Entry(const std::string& class_name) noexcept(false);

    private:
        const ::jclass reference_;

    };

    /**
     * @brief 获取类引用。如果没有则自动创建。
     */
    std::shared_ptr<Entry> Get(const std::string& class_name) noexcept(false);

    /**
     * @brief 创建类引用。用于预加载常用类。
     * @see JClassReferenceCache::Get()
     */
    void New(const std::string& class_name) noexcept(false);

private:
    std::unordered_map<std::string, std::shared_ptr<Entry>> cache_;

};

} // namespace jcpp