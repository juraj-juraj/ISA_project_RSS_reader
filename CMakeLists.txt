cmake_minimum_required (VERSION 3.0)
set (CMAKE_CXX_STANDARD 17)


project("ISA_RSS_reader")

option(RSS_READER_USE_ASAN "Compile with AddressSanitizer" OFF)
option(RSS_READER_USE_UBSAN "Compile with UndefinedBehaviorSanitizer" OFF)
option(RSS_READER_USE_MSAN "Compile with MemorySanitizer" OFF)
option(RSS_READER_USE_TSAN "Compile with ThreadSanitizer" OFF)
option(RSS_READER_ENABLE_CCACHE "Enable compiling using ccache if available" ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(PROJECT_RSS_READER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/")

# Sanitizers
set(RSS_READER_SANITIZERS "")
if (RSS_READER_USE_ASAN)
  list(APPEND RSS_READER_SANITIZERS "address")
endif ()
if (RSS_READER_USE_UBSAN)
  list(APPEND RSS_READER_SANITIZERS "undefined")
endif ()
if (RSS_READER_USE_MSAN)
  list(APPEND RSS_READER_SANITIZERS "memory")
endif ()
if (RSS_READER_USE_TSAN)
  list(APPEND RSS_READER_SANITIZERS "thread")
endif ()

if(RSS_READER_ENABLE_CCACHE)
  find_program(CCACHE ccache)
  if(CCACHE)
    message(STATUS "Found ccache: ${CCACHE}")
    set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE})
  else()
    message(STATUS "Found ccache: FALSE")
  endif()
endif()

if (RSS_READER_SANITIZERS)
  string(REPLACE ";" "," RSS_SANITIZERS_JOINED "${RSS_READER_SANITIZERS}")
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=${RSS_SANITIZERS_JOINED} -g -fno-omit-frame-pointer")
endif()

include_directories(${CMAKE_SOURCE_DIR}/src/include/)

add_subdirectory (src)

