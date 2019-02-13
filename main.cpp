#include <iostream>
#include "core/game/mini_core.h"

#ifdef _WIN64
#define RENDERER dx12
#include "renderer/dx12.h"
#else
#define RENDERER vk
#include "renderer/vk.h"
#endif


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
    trigger::abst::renderer *renderer = new trigger::renderer::RENDERER(hInstance);  
}
