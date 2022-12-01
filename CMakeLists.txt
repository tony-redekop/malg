cmake_minimum_required(VERSION 3.14)
project(malg)

add_subdirectory(test)

target_link_options(tests PRIVATE -Wall -Wextra -ldl)
target_include_directories(tests PRIVATE include)