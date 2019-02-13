#include "renderer.h"
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
            vk()
            {
                this->init();
            }
        };
    }
}
