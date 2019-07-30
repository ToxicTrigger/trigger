#include "transform.h"

trigger::transform::~transform()
{
	this->components.clear();
	this->parent = nullptr;
	this->childs.clear();
	delete parent;
}

void trigger::transform::update(float delta) noexcept
{
	for (auto& i : this->components)
	{
		if(i->active) i->update(delta);
	}
}

const int trigger::transform::get_instance_id() const
{
	return this->instance_id;
}

std::vector<trigger::component*> trigger::transform::get_components() const
{
	return this->components;
}

trigger::transform *trigger::transform::get_parent()
{
    return nullptr;
}

void trigger::transform::set_parent(trigger::transform *target)
{

}

trigger::transform *trigger::transform::get_child_at(size_t index)
{
    return nullptr;
}

auto trigger::transform::get_childs()
{
    return nullptr;
}

void trigger::transform::add_child(trigger::transform *child)
{

}

void trigger::transform::remove_parent()
{

}

void trigger::transform::remove_child(size_t index)
{

}

void trigger::transform::remove_child(trigger::transform *child)
{

}

void trigger::transform::clear_child()
{

}

void trigger::transform::clear_and_destroy_child()
{

}
