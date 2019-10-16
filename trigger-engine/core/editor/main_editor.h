#ifndef MAIN_EDITOR_H
#define MAIN_EDITOR_H

#include "impl_editor.h"
#include <string>
#include <glm/glm.hpp>
#include "../core/game/world.h"
#include "console.h"

namespace trigger::edit
{
class main_editor : public impl_editor
{
public:
	enum windows
	{
		editor,
		game_view,
		text_editor,
	};

public:
	windows current_tab = windows::editor;
	ImVec2 window_size;
	ImVec2 window_pos;
	bool is_draw_insfector = false;
	std::string TEST;
	trigger::world *world;
	int current_id = 0;
	hash_id current_selected_component;
	char *component_name;
	char *new_component_name;
	bool sel;
	trigger::transform* current_target;
	std::string* current_name;
	std::map<hash_id, trigger::component*> current_target_components;
	float thread_run;
	std::string thread_msg;
	hash_id copied_component_id;

public:
	virtual bool draw(VkDevice device, ImGui_ImplVulkanH_Window *wd)  override;
	virtual void update(float delta) override;
	void draw_objects();
	void draw_inspector();
	void draw_console();
	void draw_child(trigger::transform* vec);

	main_editor(trigger::world *world)
	{
		this->new_component_name = new char();
		this->component_name = (char*)"Please Select!";
		this->world = world;
		thread_msg = "-";
		thread_run = 0;
		trigger::tools::console::get_instance()->log("Gen");
				trigger::tools::console::get_instance()->log("Gen");
						trigger::tools::console::get_instance()->error("Gen");
								trigger::tools::console::get_instance()->warning("Gen");
												trigger::tools::console::get_instance()->log("Gen");
						trigger::tools::console::get_instance()->error("Gen");
								trigger::tools::console::get_instance()->warning("Gen");
												trigger::tools::console::get_instance()->log("Gen");
						trigger::tools::console::get_instance()->error("Gen");
								trigger::tools::console::get_instance()->warning("Gen");
			trigger::tools::console::get_instance()->log("Gen");
				trigger::tools::console::get_instance()->log("Gen");
						trigger::tools::console::get_instance()->error("Gen");
								trigger::tools::console::get_instance()->warning("Gen");
												trigger::tools::console::get_instance()->log("Gen");
						trigger::tools::console::get_instance()->error("Gen");
								trigger::tools::console::get_instance()->warning("Gen");
												trigger::tools::console::get_instance()->log("Gen");
						trigger::tools::console::get_instance()->error("Gen");
								trigger::tools::console::get_instance()->warning("Gen");
	}

	bool new_component();
	bool del_component();

	glm::fvec3 get_window_size()
	{
		return glm::fvec3(this->window_size.x, this->window_size.y, 0.0f);
	};

	glm::fvec3 get_window_position()
	{
		glm::fvec3 result(this->window_pos.x, this->window_pos.y, 0.0f);
		return result;
	};
};
} // namespace trigger::edit

#endif