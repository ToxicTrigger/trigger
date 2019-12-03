#include "engine.h"
#include "../editor/main_editor.h"
#include "mini_core.h"

#include "../../renderer/vk.h"

bool trigger::core::engine::init(int w, int h, bool edit_mod)
{
    this->state = engine_state::not_inited;	
	if (edit_mod)
	{
		this->main_editor = new trigger::edit::main_editor(&this->world);
		world.add(main_editor);
	}
    this->renderer = new trigger::rend::REND(w, h, edit_mod, this);
    this->state = engine_state::initing;
    return true;
}


int trigger::core::engine::run()
{
	return 0;
}

trigger::core::engine::~engine()
{
	delete main_editor;
}