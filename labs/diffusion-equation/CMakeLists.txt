cmake_minimum_required(VERSION 3.24)
project(diffusion_equation)

find_package(MPI REQUIRED)
set(CMAKE_CXX_STANDARD 11)

include_directories(include)

add_executable(par par.cpp)
add_executable(seq seq.cpp)

include_directories(${MPI_INCLUDE_PATH})
target_link_libraries(par
        PRIVATE m
        ${MPI_LIBRARIES}
        )

#### c71n93/matrix ####
include(FetchContent)
FetchContent_Declare(
        matrix
        GIT_REPOSITORY https://github.com/c71n93/matrix.git
        GIT_TAG 5ad7ad21755cef7f1f3bf64fb22a8f3d3c9a9b41
)
FetchContent_MakeAvailable(matrix)

include_directories(cmake-build-debug/_deps/matrix-src/include)