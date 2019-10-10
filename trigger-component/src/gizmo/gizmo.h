#pragma once
#include "../../../trigger-engine/core/game/component.h"

class gizmo : public trigger::component
{
public:
	gizmo() : trigger::component(T_CLASS)
	{
	}

	virtual gizmo* clone() const 
	{
		auto cpy_gizmo = new gizmo(*this);
		return cpy_gizmo;
	};

	virtual void update(float delta) override;
	virtual ~gizmo();
};