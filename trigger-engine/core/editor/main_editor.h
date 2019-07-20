#ifndef MAIN_EDITOR_H
#define MAIN_EDITOR_H

#include "impl_editor.h"
#include "../../ImGuiColorTextEdit/TextEditor.h"
#include <string>
#include <glm/glm.hpp>
#include "../core/game/world.h"

static TextEditor lua_editor;
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
		TextEditor::LanguageDefinition lang;
		std::string TEST;
		trigger::world* world;
		hash_id current_id = 0;
		int current_selected_component;
		char* component_name;
		char* new_component_name;
		bool sel;

	public:
		virtual bool draw() noexcept override;
		virtual void update(float delta) noexcept override;

		main_editor(trigger::world* world);

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
}

#endif