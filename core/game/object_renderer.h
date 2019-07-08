#pragma once
#include "component.h"

namespace trigger::comp
{
    class object_renderer : public trigger::component
    {
	public:
		float time;

		object_renderer() : component(T_CLASS)
		{
			SAVE_VAR_INIT(float, time, 0.0f);
		};

		virtual void update(float delta) noexcept override
		{
			time += delta;
		}
    };
};