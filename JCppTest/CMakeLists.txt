add_executable(JCppTest "JCppTest.cpp" "catch2/catch_amalgamated.hpp" "catch2/catch_amalgamated.cpp" "JClassTest.cpp" "JObjectTest.cpp")
add_compile_definitions(CATCH_AMALGAMATED_CUSTOM_MAIN)

target_link_libraries(JCppTest PRIVATE JCppLib)
