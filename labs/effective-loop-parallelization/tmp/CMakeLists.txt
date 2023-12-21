cmake_minimum_required(VERSION 3.26)
project(1e C)

find_package(MPI REQUIRED)
set(CMAKE_C_STANDARD 99)
add_compile_options(-Wall)
add_executable(seq sequential.c)

add_executable(par parallel.c)
include_directories(${MPI_INCLUDE_PATH})
target_link_libraries(par PRIVATE m  ${MPI_LIBRARIES})
if(MPI_COMPILE_FLAGS)
    set_target_properties(par PROPERTIES
            COMPILE_FLAGS "${MPI_COMPILE_FLAGS}")
    set_target_properties(seq PROPERTIES
            COMPILE_FLAGS "${MPI_COMPILE_FLAGS}")
endif()

if(MPI_LINK_FLAGS)
    set_target_properties(par PROPERTIES
            LINK_FLAGS "${MPI_LINK_FLAGS}")
    set_target_properties(seq PROPERTIES
            LINK_FLAGS "${MPI_LINK_FLAGS}")
endif()
