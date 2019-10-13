#include <iostream>
#include "core/game/mini_core.h"

static bool enable_editor = true;

#ifdef _WIN64
#include <direct.h>
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{

    char strBuffer[_MAX_PATH] = { 0, };
	char *pstrBuffer = NULL;
    std::string path(_getcwd( strBuffer, _MAX_PATH ));

    trigger::core::engine* engine = nullptr;
    if(__argc > 1) 
    {
        enable_editor = false; 
        engine = new trigger::core::engine(1280, 1080, enable_editor);
        std::string save;
        save += __argv[2];
        
        engine->object = trigger::world::load_world(path+"\\Assets\\Scene\\" + save);
        engine->editors->empty_world();
    }
    else
    {
        engine = new trigger::core::engine(1280, 1080, enable_editor);
    }
	return engine->renderer->rendering();
    //Save
    //assert(trigger::world::save_world(path+"\\Assets\\Scene", "test.txt", engine->object));
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

    auto engine = new trigger::core::engine(1280, 1080, enable_editor);
    return 0;
}
#endif