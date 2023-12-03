cmake_minimum_required(VERSION 3.26)
project(open-mp-intro)

add_executable(hello hello.cpp)
add_executable(sum sum.cpp)
add_executable(shared shared.cpp)

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(hello PUBLIC OpenMP::OpenMP_CXX)
    target_link_libraries(sum PUBLIC OpenMP::OpenMP_CXX)
    target_link_libraries(shared PUBLIC OpenMP::OpenMP_CXX)
endif()