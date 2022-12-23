cmake_minimum_required(VERSION 3.14)

set(CMAKE_BUILD_TYPE Debug)
#set(CMAKE_BUILD_TYPE Release)

project(malg)

add_subdirectory(test)

# in order to test move semantics, we need -fno-elide-constructors flag.
# this disables copy elision, a language allowed-optimization that negates
# the need to copy or move an object in certain situations, e.g. when an
# object returned from a function is immediately assigned object of same type.
# remove this flag when setting CMAKE_BUILD_TYPE to release.
target_compile_options(runtest PRIVATE -fno-elide-constructors)

target_link_options(runtest PRIVATE -Wall -Wextra -ldl)
target_include_directories(runtest PRIVATE include)