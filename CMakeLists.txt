cmake_minimum_required(VERSION 3.27)
project(compression)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/f8d7d77c06936315286eb55f8de22cd23c188571.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
        RunLengthCodecTests
        tests/RunLengthCodecTests.cpp
        tests/DictionaryCodecTests.cpp
        #benchmark/main.cpp
        benchmark/Benchmark1.cpp
)
target_link_libraries(
        RunLengthCodecTests
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(RunLengthCodecTests)
