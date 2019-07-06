#include <iostream>
#include "core/game/mini_core.h"
#include "../zinny/src/zinny.h"

static bool enable_editor = true;

#ifdef _WIN64
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
    if(strcmp(cmdLine, "-exec") == 0) enable_editor = false; 
    auto engine = new trigger::core::engine(hInstance, 800, 600, enable_editor);
	return engine->renderer->rendering();
}
#else
#define VK_VERSION_1_0 1

int main(int in, char **argv)
{
    std::string cur_exec_name = argv[0];
    std::string first_arg;
    std::vector<std::string> all_args;
    auto zin = new zinny();

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

    auto engine = new trigger::core::engine(800, 600, enable_editor);
}
#endif