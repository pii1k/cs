cmake_minimum_required(VERSION 3.16)

project(refresh_cpp CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# fetch content - gtest
include(FetchContent)
FetchContent_Declare(
  gtest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.0
)
FetchContent_GetProperties(gtest)

if(NOT gtest_POPULATED)
  FetchContent_Populate(gtest)
  set(gtest_build_tests OFF CACHE BOOL "Enable gtest tests" FORCE)
  add_subdirectory(${gtest_SOURCE_DIR} ${gtest_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

# setting executables 
set(main_file "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")
set(major_categories data_structure etc)

foreach(category IN LISTS major_categories)
  file(GLOB srcs CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${category}/*/*.cpp")
  add_executable(${category}_test
    ${srcs} 
    ${main_file}
  )
  target_link_libraries(${category}_test
    PRIVATE
      gtest
  )
endforeach()
