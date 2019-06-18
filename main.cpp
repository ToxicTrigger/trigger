#include <iostream>
#include "core/game/mini_core.h"

static bool enable_editor = true;

#ifdef _WIN64
#include <direct.h>
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
    if(strcmp(cmdLine, "-exec") == 0) enable_editor = false; 

    char strBuffer[_MAX_PATH] = { 0, };
	char *pstrBuffer = NULL;
    std::string path(getcwd( strBuffer, _MAX_PATH ));
    auto engine = new trigger::core::engine(hInstance, 800, 600, enable_editor);
    assert(trigger::world::save_world(path+"\\Assets\\Scene", "test.txt", engine->object));
	return engine->renderer->rendering();
}
#else
#define VK_VERSION_1_0 1

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
    auto engine = new trigger::core::engine(800, 600, enable_editor);
}
#endif