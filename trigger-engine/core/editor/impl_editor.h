#ifndef EDITOR_H
#define EDITOR_H
#include "../../imgui/imgui.h"
#include "../game/transform.h"
#include "../imgui/imgui_impl_vulkan.h"

namespace trigger::edit
{
	class impl_editor : public trigger::transform
	{
	public:
		// Using imgui Draw module.
		// engine->editors
		virtual bool draw(VkDevice device, ImGui_ImplVulkanH_Window *wd) = 0;

		// Update editor vals
		virtual void update(float delta) = 0;
	};
} // namespace trigger::edit

#endif