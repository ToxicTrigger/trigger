#ifndef RENDERER_H
#define RENDERER_H

namespace trigger
{
    namespace abst
    {
        class renderer
        {
        private:
            virtual void init() = 0;
            virtual void set_up() = 0;
            
        public:
        };
    } ;// namespace abst
} ;// namespace trigger

#endif