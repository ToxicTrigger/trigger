#pragma once
#include "../../../trigger-engine/core/game/component.h"
#include "../../../trigger-engine/core/game/file.h"

class material : public trigger::component
{
public:
	material() : trigger::component(T_CLASS)
	{
		trigger::property(false, trigger::property::data_type::Bool, "Test2", &this->properties);
		trigger::property(0.0f, trigger::property::data_type::Float, "Test1", &this->properties);
		trigger::property(std::string(""), trigger::property::data_type::String, "Shader", &this->properties);
	}

	virtual void update(float delta) noexcept;
	virtual ~material();
};