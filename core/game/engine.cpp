#include "engine.h"
#include "../editor/main_editor.h"
#include "mini_core.h"

#ifdef _WIN64
#include "../../renderer/dx11.h"
#else
#include "../../renderer/vk.h"
#endif

#ifndef _WIN64
bool trigger::core::engine::init(int w, int h, bool edit_mod)
{
    this->editors = new trigger::world(true);
    this->object = new trigger::world(true);
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
    auto t = new trigger::transform();
    t->name = "Jinwon";
    auto c = new trigger::transform();
    c->name = "SeungYun";
    this->object->add(t);
    this->object->add(c);
	this->main_editor = new trigger::edit::main_editor();
    this->editors->add(this->main_editor);
    this->renderer = new trigger::rend::REND(hInst, w, h, edit_mod, this);
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