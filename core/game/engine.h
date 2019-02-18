#ifndef ENGINE_H
#define ENGINE_H

#ifdef _WIN64
#define RENDERER dx12
#include "../../renderer/dx12.h"
#else
#define RENDERER vk
#include "../../renderer/vk.h"
#endif

namespace trigger
{
    namespace core
    {   
        class engine
        {
            trigger::renderer::REND *renderer;

        public:
            bool init();
            int run();
            ~engine();
        };
    } // namespace core
} // namespace trigger

#endif