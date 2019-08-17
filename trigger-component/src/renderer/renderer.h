#pragma once
#include "../../../trigger-engine/core/game/component.h"
#include "../../../trigger-engine/lib/vk/glm/glm.hpp"
#include "../../../trigger-engine/lib/vk/include/vulkan/vulkan.h"
#include <array>

struct vertex
{
	glm::vec3 pos;
	glm::vec3 color;

	static VkVertexInputBindingDescription get_binding_description()
	{
		VkVertexInputBindingDescription bindingDescription = {};
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(vertex);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		return bindingDescription;
	};

	static std::array<VkVertexInputAttributeDescription, 2> get_attribute_descriptions() 
	{
		std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions = {};
		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(vertex, pos);

		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(vertex, color);
		return attributeDescriptions;
	}
};

struct UniformBufferObject 
{
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
};

struct mesh
{
	std::vector<vertex> vertices;
	std::vector<uint16_t> indices;
	size_t buffer_id;
};

class renderer : public trigger::component
{
public:
	renderer() : trigger::component(T_CLASS)
	{
		trigger::property(std::string(""), trigger::property::data_type::String, true ,"Mesh", &properties);
	}
	
	virtual renderer* clone() const 
	{		
		auto cpy_renderer = new renderer(*this);
		return cpy_renderer;
	};

	virtual void update(float delta) override;
	virtual ~renderer();
};