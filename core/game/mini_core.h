#ifndef MINI_CORE_H
#define MINI_CORE_H
#define _CRT_SECURE_NO_WARNINGS

#include <algorithm>
#include "component_world.h"
#include "engine.h"
#include "../../tools/macros.h"

#include <glm/glm.hpp>
#include "vec.h"

namespace trigger::math
{
    typedef glm::fvec2 vec2;
    typedef glm::fvec3 vec3;
    typedef glm::fvec4 vec4;

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