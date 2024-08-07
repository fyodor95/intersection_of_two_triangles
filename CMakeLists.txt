cmake_minimum_required(VERSION 3.22.1)
project(intersection_of_two_triangles)

set(CMAKE_CXX_STANDARD 17)

add_executable(
        intersection_of_two_triangles
        src/algorithms/are_intersecting.cpp
        src/algorithms/are_nearly_equal.cpp
        src/algorithms/cross_product.cpp
        src/algorithms/determinant.cpp
        src/algorithms/dot_product.cpp
        src/main.cpp
        src/primitives/general_triangle.cpp
        src/primitives/line.cpp
        src/primitives/plane.cpp
        src/primitives/point.cpp
        src/primitives/segment.cpp
        src/primitives/triangle.cpp
        src/primitives/vector.cpp
)

target_include_directories(intersection_of_two_triangles PUBLIC include)
