#pragma once
#include "../../../trigger-engine/core/game/component.h"
#include "../../../trigger-engine/core/game/file.h"
#include "../material/material.h"
#include "../../../trigger-engine/imgui/imgui.h"
#include <stdlib.h>
#include <fstream>
#include "../../../trigger-engine/lib/vk/include/vulkan/vulkan.h"

class Shader : public trigger::component
{
public:
	static std::map<std::string, std::vector<char>> COMPILED_SHADER;
	bool init;
	bool load;
	Shader() : trigger::component(T_CLASS)
	{
		trigger::property(trigger::core::file(""), trigger::property::data_type::FilePath, "Shaders", &this->properties);
		trigger::property(false, trigger::property::data_type::Bool, false, "Compiled", &this->properties);
		trigger::property(false, trigger::property::data_type::Bool, false , "Loaded", &this->properties);
		load = false;
		init = false;
	}

	static std::vector<char> read_spv(const std::string& filename) 
	{
		std::ifstream file(filename, std::ios::ate | std::ios::binary);

		if (!file.is_open())
		{
			throw std::runtime_error("failed to open file!");
		}

		size_t fileSize = (size_t)file.tellg();
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);

		file.close();

		return buffer;
	}

	static VkShaderModule create_shader_module(VkDevice device, const std::vector<char>& compiled_spv)
	{
		VkShaderModuleCreateInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		info.codeSize = compiled_spv.size();
		info.pCode = reinterpret_cast<const uint32_t*>(compiled_spv.data());

		VkShaderModule s_module = {};
		if (vkCreateShaderModule(device, &info, nullptr, &s_module) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to Create Shader Module");
		}

		return s_module;
	}
	

	virtual void update(float delta) override;
	virtual void draw_editor() override;
	virtual Shader* clone() const override { return new Shader(*this); }

	virtual ~Shader();
};