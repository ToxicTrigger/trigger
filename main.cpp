#include <iostream>
#include "core/game/mini_core.h"

#ifdef _WIN64
#define RENDERER dx12
#include "renderer/dx12.h"
#else
#define RENDERER vk
#include "renderer/vk.h"
#endif

static bool enable_editor = true;

#ifdef _WIN64
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
    if(strcmp(cmdLine, "-exec") == 0) enable_editor = false;
    trigger::abst::renderer *renderer = new trigger::renderer::RENDERER(hInstance, enable_editor, "unlucky");  
}
#else
int main()
{
    if(strcmp(cmdLine, "-exec") == 0) enable_editor = false;
    trigger::abst::renderer *renderer = new trigger::renderer::RENDERER(hInstance, enable_editor, "unlucky");  
    return 0; 
}
#endif