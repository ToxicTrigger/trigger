#ifndef MINI_CORE_H
#define MINI_CORE_H
#define _CRT_SECURE_NO_WARNINGS

#include <algorithm>
#include "component_world.h"
#include "engine.h"
#include "../../tools/macros.h"

#include <glm/glm.hpp>
#include "vec.h"

typedef glm::fvec2 vec2;
typedef glm::fvec3 vec3;
typedef glm::fvec4 vec4;

namespace trigger::math
{
    struct vertex
    {
        vec3 p;
        vec4 color;
        vec2 uv;
    };
    
    struct triangle 
    {
        vertex p0;
        vertex p1;
        vertex p2;
    };
};

namespace trigger::colors
{
	const vec4 White = { 1.0f, 1.0f, 1.0f, 1.0f };
	const vec4 Black = { 0.0f, 0.0f, 0.0f, 1.0f };
	const vec4 Red = { 1.0f, 0.0f, 0.0f, 1.0f };
	const vec4 Green = { 0.0f, 1.0f, 0.0f, 1.0f };
	const vec4 Blue = { 0.0f, 0.0f, 1.0f, 1.0f };
	const vec4 Yellow = { 1.0f, 1.0f, 0.0f, 1.0f };
	const vec4 Cyan = { 0.0f, 1.0f, 1.0f, 1.0f };
	const vec4 Magenta = { 1.0f, 0.0f, 1.0f, 1.0f };

	const vec4 Silver = { 0.75f, 0.75f, 0.75f, 1.0f };
	const vec4 LightSteelBlue = { 0.69f, 0.77f, 0.87f, 1.0f };
}

namespace trigger::core
{
    static std::string get_path(std::string path)
    {
        
        std::string real = path;
        char from;
        char to;
    #ifdef _WIN64
        from = '/';
        to = '\\';
        std::replace( real.begin(), real.end(), from , to );
    #else
        from = '\\';
        to = '/';
        std::replace( real.begin(), real.end(), from , to );
    #endif
        return real;
    }
}

#endif