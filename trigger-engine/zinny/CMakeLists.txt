cmake_minimum_required(VERSION 3.0.0)
project(zinny VERSION 0.1.0)

include(CTest)
enable_testing()

set(HEADERS src/zinny.h src/zinny-tool.h)


add_executable(zinny main.cpp ${HEADERS})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
