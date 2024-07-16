add_executable(JCppTest "JCppTest.cpp" "catch.hpp" "JClassTest.cpp" "JObjectTest.cpp")

target_link_libraries(JCppTest PRIVATE JCppLib)
