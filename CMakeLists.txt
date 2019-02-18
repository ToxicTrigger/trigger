cmake_minimum_required(VERSION 3.0.0)
project(trigger VERSION 0.1.0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++17")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE Debug)

include(CTest)
enable_testing()

LINK_DIRECTORIES( lib/lua lib/dx12 )

set(DX12 lib/dx12/d3dx12.h )
set(LUA_HEADER lib/lua/include/lua.h lib/lua/include/lua.hpp lib/lua/include/luaconf.h lib/lua/include/lualib.h lib/lua/include/lauxlib.h )
set(TOOLS tools/macros.h )

set(IMGUI imgui/imgui.h imgui/imgui.cpp imgui/imconfig.h imgui/imgui_demo.cpp imgui/imgui_draw.cpp imgui/imgui_internal.h imgui/imgui_widgets.cpp)
set(IMGUI_VK imgui/examples/imgui_impl_glfw.h imgui/examples/imgui_impl_glfw.cpp imgui/examples/imgui_impl_vulkan.h imgui/examples/imgui_impl_vulkan.cpp)
set(IMGUI_DX imgui/examples/imgui_impl_dx12.h imgui/examples/imgui_impl_dx12.cpp)

set(REND_HEADER renderer/renderer.h renderer/renderer.cpp)
set(DX_HEADER renderer/dx12.h renderer/dx12.cpp)
set(VK_HEADER 
lib/vk/include/vulkan/vulkan.h
lib/vk/include/vulkan/vulkan.hpp
lib/vk/glfw-3.2.1.WIN64/include/GLFW/glfw3.h
renderer/vk.h 
renderer/vk.cpp
)

set(CORE_HEADER core/game/actor.h core/game/component.h core/game/component_world.h core/game/fsm.h core/game/trigger_tools.h core/game/vec.h core/game/vec.cpp core/game/engine.h)
set(MINI_CORE_HEADER core/game/mini_core.h core/game/engine.h core/game/engine.cpp)

INCLUDE_DIRECTORIES(${LUA_HEADER} ${DX12} ${REND_HEADER} ${TOOLS} ${MINI_CORE_HEADER} ${CORE_HEADER} ${IMGUI} ${IMGUI_VK} ${IMGUI_DX})

if(WIN32)
	add_executable(trigger main.cpp ${TOOLS} ${REND_HEADER} ${DX_HEADER} ${IMGUI} ${IMGUI_DX})
else()
	add_executable(trigger main.cpp 
	${TOOLS} 
	${REND_HEADER} 
	${VK_HEADER} 
	${IMGUI}
	${IMGUI_VK}
	)
endif()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

if(WIN32) # Check if we are on Windows
	if(MSVC) # Check if we are using the Visual Studio compiler
		set_target_properties(trigger PROPERTIES
			WIN32_EXECUTABLE YES
		)
		LINK_LIBRARIES( lua53 d3d12 d3dcompiler dxgi )
		target_link_libraries(trigger "d3d12.lib" "d3dcompiler.lib" "dxgi.lib" "lua53.lib")
	elseif(CMAKE_COMPILER_IS_GNUCXX)
			# SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mwindows") # Not tested
	else()
		message(SEND_ERROR "You are using an unsupported Windows compiler! (Not MSVC or GCC)")
	endif(MSVC)
elseif(APPLE)
	set_target_properties(trigger PROPERTIES
			MACOSX_BUNDLE YES
	)
	LINK_LIBRARIES( lua glfw3 glm )
	target_link_libraries(trigger "${CMAKE_SOURCE_DIR}/lib/lua/lua53.lib" "${CMAKE_SOURCE_DIR}/lib/vk/libglfw.3.3.dylib" "${CMAKE_SOURCE_DIR}/lib/vk/libvulkan.1.1.97.dylib")
elseif(UNIX)
	set_target_properties(trigger PROPERTIES
	
	)		
	LINK_LIBRARIES( lua glfw3 glm )
	target_link_libraries(trigger "${CMAKE_SOURCE_DIR}/lib/lua/lua.lib")
else()
	message(SEND_ERROR "You are on an unsupported platform! (Not Win32, Mac OS X or Unix)")
endif(WIN32)
