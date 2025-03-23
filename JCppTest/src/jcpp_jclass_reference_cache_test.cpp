#include "../catch2/catch_amalgamated.hpp"

#include <jcpp_jni_exception.h>
#include <jcpp_manager.h>

TEST_CASE("类引用缓存创建", "[jcpp_jclass_reference_cache]")
{
    jcpp::JCppManager::InitializeExplicitly();
    auto cache = jcpp::JCppManager::GetJClassReferenceCache();
    // TODO: or call GetJcrCache()

    SECTION("> 创建类引用缓存")
    {
        REQUIRE_NOTHROW(
                cache.New("java/lang/String"),
                cache.New("java/lang/Integer")
        );
    }

    SECTION("> 获取类引用缓存")
    {
        REQUIRE_NOTHROW(
                cache.Get("java/lang/String"),
                cache.Get("java/lang/Integer")
        );
    }

    SECTION("> 获取类引用缓存失败")
    {
        REQUIRE_THROWS_AS(cache.New("bad/classpath"), jcpp::exception::JniException);
        REQUIRE_THROWS_AS(cache.Get("bad/classpath"), jcpp::exception::JniException);
    }
}