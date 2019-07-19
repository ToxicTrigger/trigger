cmake_minimum_required(VERSION 3.0.0)
project(components VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set( comps 
components.cpp
)

add_library(components ${comps})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

target_compile_features(components PRIVATE cxx_range_for)