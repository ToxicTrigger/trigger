cmake_minimum_required(VERSION 3.0.0)
project(trigger VERSION 0.1.0)

set(CMAKE_BUILD_TYPE Debug)

include(CTest)
enable_testing()

LINK_DIRECTORIES( lib/lua lib/dx12 )
LINK_LIBRARIES( lua53 d3d12 d3dcompiler dxgi )

set(DX12 lib/dx12/d3dx12.h )
set(LUA_HEADER lib/lua/include/lua.h lib/lua/include/lua.hpp lib/lua/include/luaconf.h lib/lua/include/lualib.h lib/lua/include/lauxlib.h )
set(TOOLS tools/macros.h )
set(REND_HEADER renderer/renderer.h renderer/dx12.h renderer/vk.h )
set(CORE_HEADER core/game/actor.h core/game/component.h core/game/component_world.h core/game/fsm.h core/game/trigger_tools.h core/game/vec.h core/game/vec.cpp )

INCLUDE_DIRECTORIES(${LUA_HEADER} ${DX12} ${REND_HEADER} ${TOOLS} ${CORE_HEADER} )

add_executable(trigger main.cpp ${TOOLS} ${REND_HEADER} )

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
