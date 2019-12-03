cmake_minimum_required(VERSION 3.0.0)
project(components VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(../trigger-engine/lib/vk/glm ../trigger-engine/)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)

set( comps 

#DO NOT EDIT
src/material/material.h src/material/material.cpp 
src/Shader/Shader.h src/Shader/Shader.cpp 
src/camera/camera.h src/camera/camera.cpp 
src/renderer/renderer.h src/renderer/renderer.cpp 

src/gizmo/gizmo.h src/gizmo/gizmo.cpp 
components.h components.cpp
)

add_library(components ${comps})

if(WIN32) # Check if we are on Windows
	if(MSVC) # Check if we are using the Visual Studio compile
		LINK_LIBRARIES( "../trigger-engine/build/trigger-engine" )
		target_link_libraries(components "../trigger-engine/build/trigger-engine")
	elseif(CMAKE_COMPILER_IS_GNUCXX)
			# SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mwindows") # Not tested
	else()
		message(SEND_ERROR "You are using an unsupported Windows compiler! (Not MSVC or GCC)")
	endif(MSVC)
elseif(APPLE)
    LINK_LIBRARIES( "../trigger-engine/build/trigger-engine" )
    target_link_libraries(components "../trigger-engine/build/trigger-engine")
elseif(UNIX)
	set_target_properties(trigger PROPERTIES
	
	)		
	LINK_LIBRARIES( lua glfw3 glm )
	target_link_libraries(trigger "${CMAKE_SOURCE_DIR}/lib/lua/lua.lib")
else()
	message(SEND_ERROR "You are on an unsupported platform! (Not Win32, Mac OS X or Unix)")
endif(WIN32)

target_compile_features(components PRIVATE cxx_range_for)
