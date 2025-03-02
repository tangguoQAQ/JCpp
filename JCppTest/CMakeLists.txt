file(GLOB_RECURSE SRC_FILES "src/*.cpp")

add_executable(JCppTest ${SRC_FILES} "catch2/catch_amalgamated.hpp" "catch2/catch_amalgamated.cpp")
add_compile_definitions(CATCH_AMALGAMATED_CUSTOM_MAIN)

target_link_libraries(JCppTest PRIVATE JCppLib)
