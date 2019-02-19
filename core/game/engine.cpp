#include "engine.h"
#include "../editor/main_editor.h"

#ifdef _WIN64
#include "../../renderer/dx12.h"
#else
#include "../../renderer/vk.h"
#endif

#ifndef _WIN64
bool trigger::core::engine::init(int w, int h, bool edit_mod)
{
    this->renderer = new trigger::rend::REND(w,h,edit_mod);
    run();
    return true;
}
#else
bool trigger::core::engine::init(INSTANCE hInst, int w, int h, bool edit_mod)
{
    this->renderer = new trigger::rend::REND(hInst, edit_mod, "lol", this);

    this->editors = new trigger::component_world(true);
    this->editors->add(new trigger::edit::main_editor());
    
    run();
    return true;
}
#endif

int trigger::core::engine::run()
{
    this->renderer->rendering();
    return 0;
}

trigger::core::engine::~engine()
{

}