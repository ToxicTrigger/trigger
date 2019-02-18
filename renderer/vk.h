#ifndef _WIN64
#include "renderer.h"
#if REND == vk
#ifndef VK_H
#define VK_H

#include <iostream>
#define GLFW_INCLUDE_VULKAN
#include "../lib/vk/glfw-3.2.1.WIN64/include/GLFW/glfw3.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include "../lib/vk/include/vulkan/vulkan.h"

namespace trigger 
{
    namespace renderer
    {
        class vk : public trigger::abst::renderer
        {
        private:
            virtual void init() override;
            virtual void set_up() override;
            virtual void resize() override;
            virtual int rendering() override;

        public:
            vk(int w, int h, bool edit) : renderer(w,h,edit)
            {
                this->init();
                this->set_up();
            }
        };
    }
}
#endif
#endif
#endif