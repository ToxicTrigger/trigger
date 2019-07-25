#include "main_editor.h"
#include "../core/game/object_renderer.h"
#include "../components/collider.h"
#include <list>
#include <stdlib.h>
#include <fstream>
#include "../../tools/macros.h"

bool trigger::edit::main_editor::draw() noexcept
{
	{
		ImGui::Begin("Finally!");
		this->window_size = ImGui::GetWindowSize();
		this->window_pos = ImGui::GetWindowPos();
		ImGui::Text("%f : %f", window_pos.x, window_pos.y);
		ImGui::Text("%f : %f", window_size.x, window_size.y);
		ImGui::Text("Object Count : %d", this->world->get_all().size());

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

			if (ImGui::Button("New Component"))
			{
				ImGui::OpenPopup("Make a New Component");
			}

			if (ImGui::Button("Delete Component"))
			{
				ImGui::OpenPopup("Delete Component");
			}

			if (ImGui::BeginPopup("Delete Component"))
			{
				if (ImGui::BeginCombo("", this->new_component_name))
				{
					for (auto& i : *trigger::manager::class_manager::get_instance()->get_class_array())
					{
						if (ImGui::Selectable(i.first.c_str(), &this->sel))
						{
							this->new_component_name = const_cast<char*>(i.first.c_str());
						}
					}
					ImGui::EndCombo();
				}
				if (ImGui::Button("Delete"))
				{
					this->del_component();
				}
				ImGui::EndPopup();
			}

			if (ImGui::BeginPopup("Make a New Component"))
			{
				ImGui::InputText("Component Name", this->new_component_name,30);
				ImGui::TextColored(ImVec4(1.0f, 0.2f, 0.2f, 1.0f), "This action will cause the program to restart!");
				ImGui::TextColored(ImVec4(1.0f, 0.2f, 0.2f, 1.0f), "Name cannot be blank.");

				if (ImGui::Button("Let's GO!"))
				{
					auto re = std::strcmp(this->new_component_name, "");
					if (re == 0)
					{

					}
					else
					{
						this->new_component();
						std::string command;
						command.append("cd ..");
						command.append(slash);
						command.append("trigger-component & cmake --build . --config Debug");
						system(command.c_str());
						//RELOAD!
						std::exit(42);
						ImGui::CloseCurrentPopup();
					}
				}
				ImGui::EndPopup();
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
						//auto coll = GET_CLASS(trigger::comp::collider).value();
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
								int* i32 = std::any_cast<int>(&p.second.value);
								std::string text = p.second.get_string().value_or("");

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

								case trigger::property::data_type::Int:
									ImGui::Text(p.second.get_name().c_str());
									ImGui::SameLine();
									ImGui::InputInt("", i32);
									break;

								case trigger::property::data_type::String:
									ImGui::Text(p.second.get_name().c_str());
									ImGui::SameLine();
									ImGui::InputText("", const_cast<char*>(text.c_str()), 1024);
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

void trigger::edit::main_editor::update(float delta) noexcept
{

}

trigger::edit::main_editor::main_editor(trigger::world* world)
{
	this->new_component_name = new char();
	this->component_name = "Please Select!";
	lang = TextEditor::LanguageDefinition::Lua();
	lua_editor.SetLanguageDefinition(lang);
	lua_editor.SetText(this->TEST);
	this->world = world;
}
bool trigger::edit::main_editor::new_component()
{
	//Add new Component 
//todo already have??
	std::string path("mkdir ..");
	path.append(slash);
	path.append("trigger-component");
	path.append(slash);
	path.append("src");
	path.append(slash);
	path.append(this->new_component_name);
	//mkdir
	system(path.c_str());
	path.append(slash);
	path.append(this->new_component_name);
	path.append(".h");

	std::string new_name(path.substr(6, path.size()));
	new_name.replace(new_name.size() - 2, 2, ".cpp");
	//make header
	std::ofstream o(path.substr(6, path.size()));
	o << "#pragma once\n#include \"../../../trigger-engine/core/game/component.h\"\n\nclass ";
	o << this->new_component_name;
	o << " : public trigger::component\n{\npublic:\n\t";
	o << this->new_component_name;
	o << "() : trigger::component(T_CLASS)\n\t{\n\t}\n};";
	o.close();
	o.open(new_name.c_str());
	o << "#include \"";
	o << new_component_name;
	o << ".h";
	o << "\"";
	o.close();

	//read component cmakelists.txt!
	std::vector<std::string> text;
	std::string cmake;
	cmake.append("..");
	cmake.append(slash);
	cmake.append("trigger-component");
	cmake.append(slash);
	cmake.append("CMakeLists.txt");
	std::fstream f(cmake.c_str());
	if (f.is_open())
	{
		std::string line;
		while (std::getline(f, line))
		{
			text.push_back(line);
		}
	}
	f.close();

	for (int i = 0; i < text.size(); ++i)
	{
		if (text[i].find("components.cpp") != std::string::npos)
		{
			std::string name;
			name.append("src/");
			name.append(this->new_component_name);
			name.append("/");
			name.append(this->new_component_name);
			name.append(".h ");
			name.append("src/");
			name.append(this->new_component_name);
			name.append("/");
			name.append(this->new_component_name);
			name.append(".cpp ");
			text.insert(text.begin() + i, name);
			break;
		}
	}

	std::ofstream ou(cmake);
	for (auto& s : text)
	{
		ou << s;
		ou << '\n';
	}
	ou.close();

	std::vector<std::string> regi;
	std::string coms;
	coms.append("..");
	coms.append(slash);
	coms.append("trigger-component");
	coms.append(slash);
	coms.append("components.h");
	f.open(coms);
	if (f.is_open())
	{
		std::string line;
		while (std::getline(f, line))
		{
			regi.push_back(line);
		}
	}
	f.close();

	for (int i = 0; i < regi.size(); ++i)
	{
		if (regi[i].find("//DO NOT EDIT THIS FILE!") != std::string::npos)
		{
			std::string name;
			name.append("#include \"");
			name.append("src/");
			name.append(this->new_component_name);
			name.append("/");
			name.append(this->new_component_name);
			name.append(".h\"");
			regi.insert(regi.begin() + i + 1, name);
		}
		if (regi[i].find("static void reload(){") != std::string::npos)
		{
			std::string name;
			name.append("\tregi_component(");
			name.append(this->new_component_name);
			name.append(");");
			regi.insert(regi.begin() + i + 1, name);
		}
	}

	ou.open(coms);
	for (auto& s : regi)
	{
		ou << s;
		ou << '\n';
	}
	ou.close();

	return false;
}

//TODO
bool trigger::edit::main_editor::del_component()
{
	std::string path("..");
	path.append(slash);
	path.append("trigger-component");
	path.append(slash);
	path.append("src");
	path.append(slash);
	path.append(this->new_component_name);
	path.append(slash);
	path.append(this->new_component_name);

	if (remove((path + ".h").c_str()) == -1)
	{
		return false;
	}
	remove((path + ".cpp").c_str());
	std::string rm("rmdir ..");
	rm.append(slash);
	rm.append("trigger-component");
	rm.append(slash);
	rm.append("src");
	rm.append(slash);
	rm.append(this->new_component_name);
	system( rm.c_str() );

	//read component cmakelists.txt!
	std::list<std::string> text;
	std::string cmake;
	cmake.append("..");
	cmake.append(slash);
	cmake.append("trigger-component");
	cmake.append(slash);
	cmake.append("CMakeLists.txt");
	std::fstream f(cmake.c_str());
	if (f.is_open())
	{
		std::string line;
		while (std::getline(f, line))
		{
			text.push_back(line);
		}
	}
	f.close();
	for (auto&& i : text)
	{
		if (i.find(this->new_component_name) != std::string::npos)
		{
			text.remove(i);
			break;
		}
	}

	std::ofstream ou(cmake);
	for (auto& s : text)
	{
		ou << s;
		ou << '\n';
	}
	ou.close();

	std::list<std::string> regi;
	std::string coms;
	coms.append("..");
	coms.append(slash);
	coms.append("trigger-component");
	coms.append(slash);
	coms.append("components.h");
	f.open(coms);
	if (f.is_open())
	{
		std::string line;
		while (std::getline(f, line))
		{
			regi.push_back(line);
		}
	}
	f.close();

	for (auto i = regi.begin(); i != regi.end();)
	{
		if(i->find(this->new_component_name) != std::string::npos)
		{
			regi.erase(i++);
		}
		else
		{
			i++;
		}
	}

	ou.open(coms);
	for (auto& s : regi)
	{
		ou << s;
		ou << '\n';
	}
	ou.close();

	trigger::manager::class_manager::get_instance()->get_class_array()->erase(this->new_component_name);
	this->new_component_name = "";
	return true;
}
;
