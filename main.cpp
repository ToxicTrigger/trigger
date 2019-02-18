#include <iostream>
#include "core/game/mini_core.h"


static bool enable_editor = true;

#ifdef _WIN64
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
    if(strcmp(cmdLine, "-exec") == 0) enable_editor = false;
    trigger::abst::renderer *renderer = new trigger::renderer::RENDERER(hInstance, enable_editor, "unlucky");  
}
#else
int main(int in, char **argv)
{
    std::string cur_exec_name = argv[0];
    std::string first_arg;
    std::vector<std::string> all_args;

    if(in == 2)
    {
        first_arg = argv[1];
        all_args.assign(argv + 1, argv + in);
        if(all_args[0] == "-exec")
        {
            enable_editor = false;
        }
    }
    else
    {
        
    }
    auto engine = new trigger::core::engine();
    auto vk = new trigger::renderer::vk(800,600,enable_editor);
}
#endif