project(YadroTestTask)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/..)

cmake_minimum_required(VERSION 3.12)

add_executable(MainExecutable main.cpp)

add_executable(UnitTestExecutable test.cpp)
target_link_libraries(UnitTestExecutable gtest)

add_library(Methods methods.cpp)

target_link_libraries(MainExecutable Methods)
target_link_libraries(UnitTestExecutable Methods)
