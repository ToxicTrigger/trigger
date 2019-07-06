cmake_minimum_required(VERSION 3.0.0)
project(trigger VERSION 0.1.0)

if(WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE Debug)

include(CTest)
enable_testing()

LINK_DIRECTORIES( lib/lua lib/dx11 lib/vk/glm lib/vk/glm/ext core)

set(LUA_HEADER 
lib/lua/include/lua.h 
lib/lua/include/lua.hpp 
lib/lua/include/luaconf.h 
lib/lua/include/lualib.h 
lib/lua/include/lauxlib.h 
)

set(TOOLS tools/macros.h )

set(IMGUI imgui/ imgui/imgui.h imgui/imgui.cpp imgui/imconfig.h imgui/imgui_demo.cpp imgui/imgui_draw.cpp imgui/imgui_internal.h imgui/imgui_widgets.cpp)

set(IMGUI_VK 
core/imgui/imgui_impl_glfw.h  core/imgui/imgui_impl_glfw.cpp 
core/imgui/imgui_impl_vulkan.h  core/imgui/imgui_impl_vulkan.cpp
)

set(IMGUI_DX 
core/imgui/imgui_impl_win32.h  core/imgui/imgui_impl_win32.cpp 
core/imgui/imgui_impl_dx11.h  core/imgui/imgui_impl_dx11.cpp
)

set(REND_HEADER 
renderer/renderer.h 
renderer/renderer.cpp

renderer/material.h
)


set(DX_HEADER 
lib/dx11/D3DX11.h
lib/dx11/D3DX11Effect.h
renderer/dx11.h
renderer/dx11.cpp
)

set(VK_HEADER 
lib/vk/
lib/vk/include/
lib/vk/include/vulkan/vulkan.h
lib/vk/include/vulkan/vulkan.hpp
lib/vk/include/vulkan/vulkan_core.h
lib/vk/glfw-3.2.1.WIN64/include/
lib/vk/glfw-3.2.1.WIN64/include/GLFW/glfw3.h
renderer/vk.h 
renderer/vk.cpp
)

set(CORE_HEADER 
lib/vk/glm/glm.hpp
lib/vk/glm/ext/matrix_clip_space.hpp
core/game/property.h
core/game/component.h 
core/game/world.h 
core/game/fsm.h 
core/game/trigger_tools.h 
core/game/engine.h core/game/engine.cpp
core/game/file.h
core/game/mini_core.h

core/impl/impl_singleton.h
core/game/class_manager.h

core/game/object_renderer.h
core/game/transform.h core/game/transform.cpp
)

set(EDITOR_MAIN
core/editor/impl_editor.h
core/editor/main_editor.h core/editor/main_editor.cpp

ImGuiColorTextEdit/TextEditor.h ImGuiColorTextEdit/TextEditor.cpp
)

set(ZINNY 
zinny/src/zinny.h
)

INCLUDE_DIRECTORIES(${LUA_HEADER} ${DX_HEADER} ${VK_HEADER} ${REND_HEADER} ${TOOLS} ${CORE_HEADER} ${IMGUI} ${IMGUI_VK} ${IMGUI_DX} ${EDITOR_MAIN} ${ZINNY})

if(WIN32)
	add_executable(trigger main.cpp 
	${TOOLS} 
	${REND_HEADER} 
	${DX_HEADER} 
	${IMGUI} 
	${IMGUI_DX}
	${CORE_HEADER}
	${EDITOR_MAIN}
	${ZINNY}
	)
else()
	add_executable(trigger main.cpp 
	${TOOLS} 
	${REND_HEADER} 
	${VK_HEADER} 
	${IMGUI}
	${IMGUI_VK}
	${CORE_HEADER}
	${EDITOR_MAIN}
	${ZINNY}
	)
endif()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

# make shared library
ADD_LIBRARY(trigger.plugin SHARED 
core/game/transform.cpp
)

if(WIN32) # Check if we are on Windows
	if(MSVC) # Check if we are using the Visual Studio compiler
		set_target_properties(trigger PROPERTIES
			WIN32_EXECUTABLE YES
		)
		LINK_LIBRARIES( lua53 d3d11 d3dcompiler dxgi )
		target_link_libraries(trigger "d3d11.lib" "d3dx11.lib" "d3dcompiler.lib" "dxgi.lib" "lua53.lib" "Effects11d.lib")
	elseif(CMAKE_COMPILER_IS_GNUCXX)
			# SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mwindows") # Not tested
	else()
		message(SEND_ERROR "You are using an unsupported Windows compiler! (Not MSVC or GCC)")
	endif(MSVC)
elseif(APPLE)
	set(VK_ICD_FILENAMES "${CMAKE_SOURCE_DIR}/lib/vk/icd.d/MoltenVK_icd.json")
	set(VK_LAYER_PATH "${CMAKE_SOURCE_DIR}/lib/vk/explicit_layer.d")
	set_target_properties(trigger PROPERTIES
			MACOSX_BUNDLE YES)
	find_library(lua NAMES lua53.lib HINTS lib/ REQUIRED)
	find_library(glfw3 NAMES glfw3.lib REQUIRED)
	find_library(glm REQUIRED)
	LINK_LIBRARIES( ${lua} ${glfw3} ${glm} )
	target_link_libraries(trigger "${CMAKE_SOURCE_DIR}/lib/lua/lua53.lib" 
	"/usr/local/lib/libglfw.3.3.dylib" 
	"${CMAKE_SOURCE_DIR}/lib/vk/libvulkan.1.dylib"
	"${CMAKE_SOURCE_DIR}/lib/vk/libvulkan.1.1.97.dylib")
elseif(UNIX)
	set_target_properties(trigger PROPERTIES
	
	)		
	LINK_LIBRARIES( lua glfw3 glm )
	target_link_libraries(trigger "${CMAKE_SOURCE_DIR}/lib/lua/lua.lib")
else()
	message(SEND_ERROR "You are on an unsupported platform! (Not Win32, Mac OS X or Unix)")
endif(WIN32)
