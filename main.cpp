#include <iostream>
#include "core/game/mini_core.h"

#ifdef _WIN64
#define RENDERER dx12
#include "renderer/dx12.h"
#else
#define RENDERER vk
#include "renderer/vk.h"
#endif


#ifdef _WIN64
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
    trigger::abst::renderer *renderer = new trigger::renderer::RENDERER(hInstance);  
}
#else
int main()
{
    trigger::abst::renderer *renderer = new trigger::renderer::RENDERER(hInstance); 
    return 0; 
}
#endif