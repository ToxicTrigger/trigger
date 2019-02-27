#ifndef MINI_CORE_H
#define MINI_CORE_H
#define _CRT_SECURE_NO_WARNINGS

#include <algorithm>
#include "component_world.h"
#include "engine.h"
#include "../../tools/macros.h"

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