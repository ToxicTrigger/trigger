#include "engine.h"
#include "../editor/main_editor.h"
#include "mini_core.h"

#include "../../renderer/vk.h"


#ifndef _WIN64
bool trigger::core::engine::init(int w, int h, bool edit_mod)
{
    this->editors = new trigger::world(true);
    this->object = new trigger::world(true);
    this->object->name = "SceneObjects";
    auto tmp = new trigger::transform();
    tmp->add_component(new trigger::comp::object_renderer());
    this->object->add(tmp);
    object->save_world("..","my.toml", object);
    this->editors->add(new trigger::edit::main_editor());
    this->renderer = new trigger::rend::REND(w,h,edit_mod,this);
    return true;
}
#else
bool trigger::core::engine::init(INSTANCE hInst, int w, int h, bool edit_mod)
{
    this->state = engine_state::not_inited;	

    this->editors = new trigger::world(true);
	this->object = new trigger::world(true);
    this->object->name = "SceneObjects";
	this->main_editor = new trigger::edit::main_editor(this->object);
    this->editors->add(this->main_editor);
    this->renderer = new trigger::rend::REND(w, h, edit_mod, this);
    this->state = engine_state::initing;
    return true;
}
#endif

int trigger::core::engine::run()
{
	return 0;
}

trigger::core::engine::~engine()
{
}