#include "main_editor.h"
#include "../core/game/object_renderer.h"
#include "../components/collider.h"

bool trigger::edit::main_editor::draw() noexcept
{
	{
		ImGui::Begin("Finally!");
		this->window_size = ImGui::GetWindowSize();
		this->window_pos = ImGui::GetWindowPos();
		ImGui::Text("%f : %f", window_pos.x, window_pos.y);
		ImGui::Text("%f : %f", window_size.x, window_size.y);

		ImGui::BeginTabBar("Test");
		if (ImGui::BeginTabItem("Editor View"))
		{
			current_tab = windows::editor;
			if (ImGui::Button("Create Object"))
			{
				auto tmp = new trigger::transform();
				this->world->add(tmp);
			}

			if (ImGui::Button("Add Component"))
			{
				ImGui::OpenPopup("Select Component");
			}

			if (ImGui::BeginPopup("Select Component"))
			{
				ImGui::BeginGroup();

				if (ImGui::BeginCombo("", this->component_name))
				{
					for (auto& i : *trigger::manager::class_manager::get_instance()->get_class_array())
					{
						if (ImGui::Selectable(i.first.c_str(), &this->sel))
						{
							this->component_name = const_cast<char*>(i.first.c_str());
						}
					}
					ImGui::EndCombo();
				}
				if (ImGui::Button("Add"))
				{
					auto objs = this->world->get_all();

					if (this->current_id != 0)
					{
						auto comps = objs[this->current_id];
						auto tmp = trigger::manager::class_manager::get_instance()->get_class_array()->at(this->component_name);
						comps->add_component(tmp);
						ImGui::CloseCurrentPopup();
					}
				}

				ImGui::EndGroup();
				ImGui::EndPopup();
			}


			{
				auto objs = this->world->get_all();

				if (this->current_id != 0)
				{
					ImGui::Text("%s", objs[current_id]->get_name().c_str());
					ImGui::Text("Instance ID : %d", this->current_id);
					auto comps = objs[this->current_id]->get_components();
					for (auto c : comps)
					{
						ImGui::Separator();
						ImGui::Text(c->get_type_name().c_str());
						for (auto p : c->properties)
						{
							if (p.second.controllable)
							{
								float f = p.second.get_float().value_or(0);
								bool* b = std::any_cast<bool>(&p.second.value);
								switch (p.second.type)
								{
								case trigger::property::data_type::Bool:
									ImGui::Text(p.second.get_name().c_str());
									ImGui::SameLine();
									ImGui::Checkbox("", b);
									break;

								case trigger::property::data_type::Float:
									ImGui::Text(p.second.get_name().c_str());
									ImGui::SameLine();
									ImGui::InputFloat("", &f);
									break;
								}
							}
						}
						ImGui::Separator();
					}
				}
			}

			ImGui::EndTabItem();
		}
		static bool hello;

		if (ImGui::BeginTabItem("Game View"))
		{
			current_tab = windows::game_view;
			ImGui::Checkbox("Hello", &hello);
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Lua View"))
		{
			current_tab = windows::text_editor;
			lua_editor.Render("TEST");

			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
		ImGui::End();
	}

	return true;
};

void trigger::edit::main_editor::update(float delta) noexcept {

}
trigger::edit::main_editor::main_editor(trigger::world* world)
{
	this->component_name = "Please Select!";
	lang = TextEditor::LanguageDefinition::Lua();
	lua_editor.SetLanguageDefinition(lang);
	lua_editor.SetText(this->TEST);
	this->world = world;
	trigger::component::regi_component<trigger::transform>();
	trigger::component::regi_component<trigger::comp::object_renderer>();
	trigger::component::regi_component<trigger::comp::collider>();
};