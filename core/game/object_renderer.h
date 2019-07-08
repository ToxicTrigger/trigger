#pragma once
#include "component.h"

namespace trigger::comp
{
    class object_renderer : public trigger::component
    {
	public:
		trigger::property time;

		object_renderer() : component(T_CLASS)
		{
			time = trigger::property(0.0f, property::data_type::Float, "time", &this->properties);
		};

		virtual void update(float delta) noexcept override
		{
			time.value = std::any_cast<float>(time.value) + delta;
		}
    };
};