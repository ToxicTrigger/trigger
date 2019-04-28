#pragma once
#include "component.h"

namespace trigger::comp
{
    class object_renderer : public trigger::component
    {
	public:
		float time;
		object_renderer() : component()
		{
			time = 0;
		};

		virtual void update(float delta) noexcept override
		{
			time += delta;
		}
    };
};