add_executable(JCppTest "$old/JCppTest.cpp" "catch2/catch_amalgamated.hpp" "catch2/catch_amalgamated.cpp" "$old/JClassTest.cpp" "$old/JObjectTest.cpp" "jcpp_manager_test.cpp")
add_compile_definitions(CATCH_AMALGAMATED_CUSTOM_MAIN)

target_link_libraries(JCppTest PRIVATE JCppLib)
