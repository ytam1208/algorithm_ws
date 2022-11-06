cmake_minimum_required(VERSION 3.10)
project(code_test)

include_directories(
 include
#  ${CERES_INCLUDE_DIRS}
)

set(CMAKE_CXX_STANDARD 17)

add_executable(test src/main.cpp)

add_executable(inline src/inline.cpp)

add_executable(class_new src/class_new.cpp)

add_executable(virtual_class src/virtual_class.cpp)

add_executable(unique_ptr src/unique_ptr.cpp)

# add_executable( src/unique_ptr.cpp)