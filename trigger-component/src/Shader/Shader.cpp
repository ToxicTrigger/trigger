#include "Shader.h"

void Shader::update(float delta)
{
}

void Shader::draw_editor()
{
	init = this->properties.at(hash_str("Compiled")).get<bool>();
	std::string text = init ? "Compiled" : "Nope";
	if (init)
	{
		ImGui::TextColored(ImVec4(0.3f, 0.8f, 0.5f, 1.0f), text.c_str());
	}
	else
	{
		ImGui::TextDisabled(text.c_str());
	}

	if (ImGui::Button("Compile"))
	{
		this->set_property("Compiled", true);
	}
}

Shader::~Shader()
{
};