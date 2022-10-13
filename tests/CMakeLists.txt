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
add_compile_definitions(TEST_DIRECTORY="${CMAKE_SOURCE_DIR}/tests")

add_executable(argParserTest
    argParserTest.cpp)

target_link_libraries(argParserTest
 PRIVATE
  GTest::GTest
  parameterParser)

add_executable(urlParserTest
    urlParserTest.cpp)

target_link_libraries(urlParserTest
    PRIVATE
    GTest::GTest
    URLParser)

add_test(NAME argParserGTest COMMAND "${CMAKE_BINARY_DIR}/argParserTest")
add_test(NAME urlParserGTest COMMAND "${CMAKE_BINARY_DIR}/urlParserTest")
