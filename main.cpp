#include <iostream>
#include "renderer/dx12.h"
#include "renderer/vk.h"
#include "core/game/actor.h"

#ifdef _WIN64
#define RENDERER "dx12"
#else
#define RENDERER "vk"
#endif

int main(int, char**) 
{
    trigger::abst::renderer *renderer;
    if(RENDERER == "dx12")
    {
        renderer = new trigger::renderer::dx12();
    }
    else
    {
        renderer = new trigger::renderer::vk();
    } 

};
