file(GLOB_RECURSE SRC_FILES "src/*.cpp")
file(GLOB_RECURSE INCLUDE_FILES "include/*.h")

add_library(JCppLib STATIC ${SRC_FILES} ${INCLUDE_FILES})

target_include_directories(JCppLib PRIVATE "include/")
