cmake_minimum_required(VERSION 3.27)
project(triangle_intersections)

set(CMAKE_CXX_STANDARD 20)

set(cinolib_DIR <YOUR CINOLIB PATH HERE>)


set(CINOLIB_USES_OPENGL_GLFW_IMGUI    ON  ) # used for rendering with OpenGL
set(CINOLIB_USES_SHEWCHUK_PREDICATES  ON  ) # used for exact geometric tests on input points
set(CINOLIB_USES_INDIRECT_PREDICATES  ON ) # used for exact geometric tests on implicit points
set(CINOLIB_USES_CGAL_GMP_MPFR        ON ) # used for rational numbers with lazy kernel

set(CGAL_DO_NOT_WARN_ABOUT_CMAKE_BUILD_TYPE TRUE)

find_package(cinolib REQUIRED)
find_package(CGAL REQUIRED)
add_executable(
        ${PROJECT_NAME} main.cpp
        orientTemplated.h
        tri_tri_intersect.h
        printIntersections.cpp
        printIntersections.h

)

target_link_libraries(${CMAKE_PROJECT_NAME} CGAL::CGAL cinolib)
