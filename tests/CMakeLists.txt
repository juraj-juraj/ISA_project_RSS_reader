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

add_executable(utilsTest
    utilsTest.cpp)

target_link_libraries(utilsTest
    PRIVATE
    GTest::GTest
    feedreaderUtils
)

add_executable(feedDownloaderTest
    feedDownloaderTest.cpp)

target_link_libraries(feedDownloaderTest
    PRIVATE
    GTest::GTest
    feedDownloader
)

add_test(NAME argParserGTest COMMAND "${CMAKE_BINARY_DIR}/argParserTest")
add_test(NAME urlParserGTest COMMAND "${CMAKE_BINARY_DIR}/urlParserTest")
add_test(NAME utilsGTest COMMAND "${CMAKE_BINARY_DIR}/utilsTest")
add_test(NAME feedDownloaderGTest COMMAND "${CMAKE_BINARY_DIR}/feedDownloaderTest")

add_test(NAME integration_tests COMMAND "${CMAKE_SOURCE_DIR}/tests/integration_tests.sh" "${CMAKE_BINARY_DIR}/feedreader" "${CMAKE_SOURCE_DIR}/tests")

