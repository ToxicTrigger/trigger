#pragma once
#include "../../../trigger-engine/core/game/component.h"
#include "../../../trigger-engine/core/game/transform.h"
#include "../../../trigger-engine/lib/vk/include/vulkan/vulkan.h"


class camera : public trigger::component
{
public:
	static component* Main_Camera;

	camera() : trigger::component(T_CLASS)
	{
		trigger::property(800.0f, trigger::property::data_type::Float, "Width", &this->properties);
		trigger::property(640.0f, trigger::property::data_type::Float, "Height", &this->properties);
		trigger::property(0.0f, trigger::property::data_type::Float, "MinDepth", &this->properties);
		trigger::property(1.0f, trigger::property::data_type::Float, "MaxDepth", &this->properties);
		trigger::property(60.0f, trigger::property::data_type::Float, "FoV", &this->properties);
		trigger::property(false, trigger::property::data_type::Bool, "Orthogonal", &this->properties);
	}

	virtual camera* clone() const override
	{ 
		auto copy_camera = new camera(*this);

		return copy_camera;
	};

	virtual void update(float delta) override;
	virtual ~camera();
};