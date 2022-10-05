include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.11.0
)
FetchContent_MakeAvailable(googletest)
add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)

include_directories(${CMAKE_SOURCE_DIR}/tests/)

add_executable(argParserTest
    argParserTest.cpp)

target_link_libraries(argParserTest
 PRIVATE
  GTest::GTest
  parameterParser)

add_test(NAME argParserGTest COMMAND "${CMAKE_BINARY_DIR}/argParserTest")
