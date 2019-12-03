#pragma once
#include "../../../trigger-engine/core/game/component.h"

class gizmo : public trigger::component
{
public:
	gizmo() : trigger::component(T_CLASS)
	{
		trigger::property(.0f, trigger::property::data_type::Float, true, "TEST", &this->properties);
	}

	virtual gizmo* clone()  const override
	{
		auto cpy_gizmo = new gizmo(*this);
		return cpy_gizmo;
	};

	virtual void update(float delta) override;
	virtual ~gizmo();
};