#ifndef ENGINE_H
#define ENGINE_H

#include "../../renderer/renderer.h"
#include "component_world.h"
#include "mini_core.h"

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
        enum engine_state
        {
            not_inited,
            initing,
            inited,
            
            awake,
            start,
            run,
            paused,

            stop,
            clean_up,
            shut_down
        };

        class engine
        {
        protected:
            trigger::core::engine_state state;

        public:
            trigger::rend::renderer *renderer;
            trigger::component_world *editors;

        public:
#ifdef _WIN64
            engine(INSTANCE hInst, int w, int h, bool edit_mod)
            {
                this->init(hInst, w, h, edit_mod);
                this->state = engine_state::inited;
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
            engine_state get_state() const
            {
                return this->state;
            }

            ~engine();
        };
    } // namespace core
} // namespace trigger

#endif