#include <iostream>
#include "core/game/mini_core.h"
#include "../zinny/src/zinny.h"

static bool enable_editor = true;

class test
{

private :
    int c = 2;

public:
    int a = 0;
    int b = 0;  
};

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
        engine = new trigger::core::engine(hInstance, 800, 600, enable_editor);
        std::string save;
        save += __argv[2];
        
        engine->object = trigger::world::load_world(path+"\\Assets\\Scene\\" + save);
        engine->editors->empty_world();
    }
    else
    {
        trigger::transform* a, *b;
        a = new trigger::transform();
        a->set_name("dd");
        auto s = cast<char*>(a);
        b = cast<trigger::transform*>(s);
        auto aaa = trigger::world::load_world(path+"\\Assets\\Scene\\test.toml");
        engine = new trigger::core::engine(hInstance, 800, 600, enable_editor);

        engine->object->add(b);
        assert(trigger::world::save_world(path+"\\Assets\\Scene", "test2.map", engine->object));
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