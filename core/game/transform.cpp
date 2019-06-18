#include "transform.h"

trigger::transform::~transform()
{
	this->components.clear();
}

void trigger::transform::update(float delta) noexcept
{
	for (auto i : this->components)
	{
		if(i->active) i->update(delta);
	}
}

const int trigger::transform::get_instance_id() const
{
	return this->hash_code;
}

auto trigger::transform::get_components() const
{
	return this->components;
}
