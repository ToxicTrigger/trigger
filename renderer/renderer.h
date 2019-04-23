#ifndef RENDERER_H
#define RENDERER_H

#include "../tools/macros.h"
#include "../core/game/engine.h"

namespace trigger
{
    namespace core
    {
        class engine;
    }

    namespace rend
    {
        enum window_state
        {
            Minimized,
            Maximized,
            Resizeing,
        };

        class renderer
        {
        protected:
            window_state window_mode;
            
        public:
            virtual int init() = 0;
            virtual void set_up() = 0;
            virtual int rendering() = 0;
            virtual void resize() = 0;

        public:
            trigger::core::engine* engine;

        public:
            renderer(int width, int height, bool edit_mode, trigger::core::engine* engine) : 
            mClientWidth(width), mClientHeight(height), edit_mode(edit_mode), engine(engine)
            {};

            virtual ~renderer() {}
            int mClientWidth, mClientHeight;
            bool edit_mode;
        };
    } ;// namespace abst
} ;// namespace trigger

#endif