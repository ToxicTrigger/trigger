#include "renderer.h"
#if REND == vk
#include <iostream>

namespace trigger 
{
    namespace renderer
    {
        class vk : public trigger::abst::renderer
        {
        private:
            virtual void init()
            {
                std::cout << "init vk  ... " << std::endl;
            } 

            virtual void set_up()
            {

            }

        public:
            vk(int w, int h, bool edit) : renderer(w,h,edit)
            {
                this->init();
            }
        };
    }
}
#endif