cmake_minimum_required(VERSION 3.24)
project(diffusion_equation)

find_package(MPI REQUIRED)
set(CMAKE_CXX_STANDARD 17)

include_directories(include)
include_directories(third-party/include)
## ! hardcode ! result of running "python3-config --include"
#include_directories(/opt/homebrew/opt/python@3.11/Frameworks/Python.framework/Versions/3.11/include/python3.11)
#include_directories(/opt/homebrew/lib/python3.11/site-packages/numpy/core/include)

add_executable(parc parc.cpp)
add_executable(seq seq.cpp)

include_directories(${MPI_INCLUDE_PATH})
target_link_libraries(parc
        PRIVATE m
        ${MPI_LIBRARIES}
        )

#### c71n93/matrix ####
include(FetchContent)
FetchContent_Declare(
        matrix
        GIT_REPOSITORY https://github.com/c71n93/matrix.git
        GIT_TAG 92a34e60b6611783332390bae4c3faba84f84ca9 # latest commit
)
FetchContent_MakeAvailable(matrix)

include_directories(cmake-build-debug/_deps/matrix-src/include)