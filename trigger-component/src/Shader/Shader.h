#pragma once
#include "../../../trigger-engine/core/game/component.h"
#include "../../../trigger-engine/core/game/file.h"
#include "../material/material.h"
#include "../../../trigger-engine/imgui/imgui.h"

class Shader : public trigger::component
{
public:
	bool init;
	Shader() : trigger::component(T_CLASS)
	{
		trigger::property(trigger::core::file(""), trigger::property::data_type::FilePath, "Shaders", &this->properties);
		trigger::property(false, trigger::property::data_type::Bool, false , "Compiled", &this->properties);
		init = false;
	}

	virtual void update(float delta) override;
	virtual void draw_editor() override;
	virtual Shader* clone() const { return new Shader(*this); }

	virtual ~Shader();
};