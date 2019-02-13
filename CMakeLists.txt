cmake_minimum_required(VERSION 3.0.0)
project(trigger VERSION 0.1.0)

include(CTest)
enable_testing()

set(TOOLS tools/macros.h )
set(REND_HEADER renderer/renderer.h renderer/dx12.h renderer/vk.h )


INCLUDE_DIRECTORIES(${REND_HEADER} ${TOOLS})

add_executable(trigger main.cpp ${TOOLS} ${REND_HEADER} )

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
