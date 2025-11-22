cmake_minimum_required(VERSION 3.14)
project(SinglyLinkedListProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

include_directories(include)

add_executable(demo src/main.cpp)

add_executable(run_tests
    tests/test_list.cpp
    tests/test_resource.cpp
)

target_link_libraries(run_tests
    PRIVATE
        GTest::gtest_main
        GTest::gmock
        Threads::Threads
)

target_include_directories(run_tests
    PRIVATE
        include
)

add_test(NAME SinglyLinkedListTests COMMAND run_tests)