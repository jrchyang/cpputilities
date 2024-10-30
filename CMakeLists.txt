# 指定 cmake 最小版本
cmake_minimum_required(VERSION 3.5)

# 指定项目名称
project(cpputilities
  VERSION 0.0.1
  LANGUAGES CXX C)

# 指定适用的操作系统
if(NOT CMAKE_SYSTEM_NAME STREQUAL "Linux")
  message(FATAL_ERROR "This project requires Linux operating system.")
endif()

# 设置编译提醒
if(NOT CMAKE_BUILD_TYPE AND EXISTS "${CMAKE_SOURCE_DIR}/.git")
  set(default_build_type "Debug")
  set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
      STRING "Default BUILD_TYPE is Debug, other options are: RelWithDebInfo, Release, and MinSizeRel." FORCE)
endif()

# 设置 C++ 标准为 C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CheckCXXCompilerFlag)

# 指定成果物路径
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

# 指定文件搜索路径
include_directories(${CMAKE_SOURCE_DIR}/include)

# 设置库的源文件
set(SOURCES
  src/abort.cpp
)

# 创建动态库
add_library(cpputilities SHARED ${SOURCES})

# 创建静态库
add_library(cpputilities-static STATIC ${SOURCES})

# 安装头文件
install(DIRECTORY include/ DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/cpputilities"
        FILES_MATCHING PATTERN "*.hpp" PATTERN "*.h")

# 安装库文件
install(TARGETS cpputilities cpputilities-static
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin)

# Used by googletest.
check_cxx_compiler_flag(-Wno-missing-field-initializers
                        CPPUTILITIES_HAVE_NO_MISSING_FIELD_INITIALIZERS)
check_cxx_compiler_flag(-Wno-implicit-int-float-conversion
                        CPPUTILITIES_HAVE_NO_IMPLICIT_INT_FLOAT_CONVERSION)

option(CPPUTILITIES_BUILD_TESTS "Build cpputilities's own tests." ON)
if(CPPUTILITIES_BUILD_TESTS)

  enable_testing()

  # This project is tested using GoogleTest.
  add_subdirectory("third_party/googletest")

  # GoogleTest triggers a missing field initializers warning.
  if(CPPUTILITIES_HAVE_NO_MISSING_FIELD_INITIALIZERS)
    set_property(TARGET gtest
        APPEND PROPERTY COMPILE_OPTIONS -Wno-missing-field-initializers)
    set_property(TARGET gmock
        APPEND PROPERTY COMPILE_OPTIONS -Wno-missing-field-initializers)
  endif(CPPUTILITIES_HAVE_NO_MISSING_FIELD_INITIALIZERS)

  if(CPPUTILITIES_HAVE_NO_IMPLICIT_INT_FLOAT_CONVERSION)
    set_property(TARGET gtest
        APPEND PROPERTY COMPILE_OPTIONS -Wno-implicit-int-float-conversion)
  endif(CPPUTILITIES_HAVE_NO_IMPLICIT_INT_FLOAT_CONVERSION)

  add_executable(cpputilities_unittest "")
  target_sources(cpputilities_unittest
    PRIVATE
      "test/test_macros.cpp"
  )
  target_link_libraries(cpputilities_unittest cpputilities gmock_main gtest)

  add_test(
    NAME cpputilities_unittest
    WORKING_DIRECTORY "${EXECUTABLE_OUTPUT_PATH}"
    COMMAND "${EXECUTABLE_OUTPUT_PATH}/cpputilities_unittest")

endif(CPPUTILITIES_BUILD_TESTS)
