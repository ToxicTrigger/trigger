#include "transform.h"

trigger::transform::transform(vec3 pos, vec3 scale, vec3 rot, std::string name)
{
	this->hash_code = make_hash_code();
	this->real_position = pos;
	this->real_scale = scale;
	this->real_rotation = rot;
	this->name = name;
	this->components = std::vector<trigger::component*>();
}

trigger::transform::~transform()
{
	this->components.clear();
}

void trigger::transform::update(float delta)
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
