#pragma once
#include "../../../trigger-engine/core/game/component.h"


class material : public trigger::component
{
public:
	material() : trigger::component(T_CLASS)
	{
		trigger::property(0, trigger::property::data_type::HashID, "Vert Shader", &this->properties);
		trigger::property(0, trigger::property::data_type::HashID, "Frag Shader", &this->properties);
	}

	virtual void update(float delta);
	virtual ~material();
	virtual material* clone() const
	{
		return new material(*this);
	}
};