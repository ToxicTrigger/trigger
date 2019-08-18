#include "Shader.h"
std::map<std::string, std::vector<char>> Shader::COMPILED_SHADER = std::map<std::string, std::vector<char>>();

void Shader::update(float delta)
{
}

void Shader::draw_editor()
{
	init = this->properties.at(hash_str("Compiled")).get<bool>();
	load = this->properties.at(hash_str("Loaded")).get<bool>();

	std::string text = init ? "Compiled" : "Nope";
	if (init)
	{
		ImGui::TextColored(ImVec4(0.3f, 0.8f, 0.5f, 1.0f), text.c_str());
	}
	else
	{
		ImGui::TextDisabled(text.c_str());
	}

	std::string load_msg = init ? "Success" : "Nope";
	if (load)
	{
		ImGui::TextColored(ImVec4(0.3f, 0.8f, 0.5f, 1.0f), load_msg.c_str());
	}
	else
	{
		ImGui::TextDisabled(load_msg.c_str());
	}

	if (ImGui::Button("Compile"))
	{
		auto file = this->properties[hash_str("Shaders")].get<trigger::core::file>();
		file.update_date();
		this->set_property("Shaders", file);
		if ( file.get_type() == trigger::core::file::hlsl)
		{
#ifdef _WIN64
			std::string command("lib\\vk\\glslc.exe ");
			auto me = (command + *file.get_path() + " -o " + *file.get_path() + ".spv");
			int err = system((command + *file.get_path() + " -o " + *file.get_path() + ".spv").c_str());
			if(err == 0) this->set_property("Compiled", true);
#else
			std::string command("lib/vk/glslc ");
			auto me = (command + *file.get_path() + " -o " + *file.get_path() + ".spv");
			int err = system((command + *file.get_path() + " -o " + *file.get_path() + ".spv").c_str());
			if (err == 0) this->set_property("Compiled", true);
#endif	
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("Load"))
	{
		auto file = this->properties[hash_str("Shaders")].get<trigger::core::file>();
		auto spv = std::string(*file.get_path());
		spv.append(".spv");
		Shader::COMPILED_SHADER.insert(std::pair < std::string, std::vector<char>>(spv, read_spv(spv)));
		this->set_property("Loaded", true);
	}

}

Shader::~Shader()
{
};