cmake_minimum_required(VERSION 3.10)
project(code_test)

include_directories(
 include
#  ${CERES_INCLUDE_DIRS}
)

set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

# add_executable(test src/main.cpp)

# add_executable(inline src/inline.cpp)

# add_executable(class_new src/class_new.cpp)

# add_executable(virtual_class src/virtual_class.cpp)

# add_executable(unique_ptr src/unique_ptr.cpp)

# add_executable(shared_ptr src/shared_ptr.cpp)

# add_executable(using_function_pointer src/using_function_pointer.cpp)

# add_executable(while_loop src/while_loop_clock_test.cpp)

# add_executable(shrink src/shrink_to_fit.cpp)

# add_executable(auto src/auto.cpp)

# add_executable(while src/while_loop_clock_test.cpp)

# add_executable(bind src/bind.cpp)

# add_executable(function src/function.cpp)

# add_executable(list src/list.cpp)

# add_executable(unordered_map src/unordered_map.cpp)

# add_executable(Naive_Singleton src/Morden_Design/Naive_Singleton.cpp)

# add_executable(Smart_Singleton src/Morden_Design/Smart_Singleton.cpp)

add_executable(union src/union.cpp)