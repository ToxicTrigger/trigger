#include "transform.h"

trigger::transform::~transform()
{
	trigger::component::~component();
	this->components.clear();
	this->parent = nullptr;
	this->childs.clear();
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
