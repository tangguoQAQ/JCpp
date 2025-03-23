cmake_minimum_required (VERSION 3.8)

# 如果支持，请为 MSVC 编译器启用热重载。
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project("JCpp" VERSION 0.1 DESCRIPTION "A C++ object-oriented wrapper for JNI" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 检查 JAVA_HOME 环境变量是否设置。
if(NOT DEFINED ENV{JAVA_HOME})
    message(FATAL_ERROR "JAVA_HOME environment variable is not set")
endif()

# include_directories("$ENV{JAVA_HOME}/include")
# if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
#   include_directories("$ENV{JAVA_HOME}/include/win32")
# elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
#   include_directories("$ENV{JAVA_HOME}/include/linux")
# endif()
# include_directories("${PROJECT_SOURCE_DIR}/JCppLib/include")
# link_libraries("$ENV{JAVA_HOME}/lib/jvm.lib")

find_package(JNI REQUIRED)  

include_directories(${JNI_INCLUDE_DIRS})  
link_libraries(${JNI_LIBRARIES})  

enable_testing()

# 包含子项目。
add_subdirectory("JCppLib")
add_subdirectory("JCppTest")

# 启用编译时 clang-tidy 检查。
set(CMAKE_CXX_CLANG_TIDY "clang-tidy" "-config-file=.clang-tidy")
