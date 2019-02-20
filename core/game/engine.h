#ifndef ENGINE_H
#define ENGINE_H

#include "../../renderer/renderer.h"
#include "component_world.h"

#ifdef _WIN64
#include <windows.h>
#endif

namespace trigger
{
    namespace rend
    {
        class renderer;
    }

    namespace core
    {   
        class engine
        {
        public:
            trigger::rend::renderer *renderer;
            trigger::component_world *editors;

        public:
#ifdef _WIN64
            engine(INSTANCE hInst, int w, int h, bool edit_mod)
            {
                this->init(hInst, w, h, edit_mod);
                this->run();
            }
            bool init(INSTANCE hInst, int w, int h, bool edit_mod);
#else
            engine(int w, int h, bool edit_mod)
            {
                this->init(w, h, edit_mod);
            }
            bool init(int w, int h, bool edit_mod);
#endif
            int run();
            ~engine();
        };
    } // namespace core
} // namespace trigger

#endif