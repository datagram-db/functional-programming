cmake_minimum_required(VERSION 3.13)
project(skeleton)

set(CMAKE_CXX_STANDARD 17)

add_executable(skeleton main.cpp include/languageStyleIterator/javaIterators.h include/languageStyleIterator/cppIterators.h include/functionalIterator.h)