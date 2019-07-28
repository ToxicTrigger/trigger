#include "main_editor.h"
#include "../core/game/object_renderer.h"
#include <list>
#include <stdlib.h>
#include <fstream>
#include "../../tools/macros.h"
#include "../../imgui/misc/cpp/imgui_stdlib.h"

bool trigger::edit::main_editor::draw() noexcept
{
	static bool opt_fullscreen_persistant = true;
	bool opt_fullscreen = opt_fullscreen_persistant;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}

	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
	// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
	// all active windows docked into it will lose their parent and become undocked.
	// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
	// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", 0, window_flags);
	ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);
	// DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Objects"))
		{

			if (ImGui::MenuItem("Create New Object"))
			{
				auto tmp = new trigger::transform();
				this->world->add(tmp);
			}
			if (ImGui::MenuItem("Add Component"))
			{
				ImGui::OpenPopup("Select Component");
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	ImGui::End();


	draw_inspector();
	draw_objects();
	draw_console();

	{
		ImGui::Begin("Finally!");

		ImGui::BeginTabBar("Test");
		if (ImGui::BeginTabItem("Editor View"))
		{
			current_tab = windows::editor;
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

void trigger::edit::main_editor::draw_objects()
{
	ImGui::Begin("Objects");
	for (auto& obj : this->world->get_objects<trigger::transform>())
	{
		if (ImGui::Selectable(obj->get_name()->c_str()))
		{
			this->current_id = obj->get_instance_id();
		}
	}
	ImGui::End();
}

void trigger::edit::main_editor::draw_inspector()
{
	ImGui::Begin("inspector");

	if (this->current_id != 0)
	{
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

		{
			if (ImGui::BeginPopup("Select Component"))
			{
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
						auto tmp = new trigger::component(*trigger::manager::class_manager::get_instance()->get_class_array()->at(this->component_name));
						comps->add_component(tmp);
						ImGui::CloseCurrentPopup();
					}
				}
				ImGui::EndPopup();
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
				ImGui::InputText("Component Name", this->new_component_name, 30);
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
#ifdef _WIN64
						command.append("cd ..");
						command.append(slash);
						command.append("trigger-component & cmake --build . --config Debug");
#else
						command.append("cd trigger-component & cmake --build . --config Debug");
#endif
						system(command.c_str());
						//RELOAD!
						//std::exit(42);
						ImGui::CloseCurrentPopup();
					}
				}
				ImGui::EndPopup();
			}
		}
		
		auto object = this->world->get_all()[current_id];
		ImGui::InputText("", object->get_name());
		ImGui::Text("Instance ID : %d", object->get_instance_id());
		auto comps = object->get_components();
		for (auto* c : comps)
		{
			ImGui::Separator();
			ImGui::Text(c->get_type_name().c_str());

			for (auto p : c->properties)
			{
				if (p.second.controllable)
				{
					ImGui::Text(p.second.get_name().c_str());
					ImGui::SameLine();
					if (p.second.type == trigger::property::data_type::Bool)
					{
						bool* b = std::any_cast<bool>(&p.second.value);
						if (b != nullptr && ImGui::Checkbox("", b))
						{
							c->set_property(p.second.get_name(), *b);
						}
					}
					else if (p.second.type == trigger::property::data_type::Double)
					{
						double* d = std::any_cast<double>(&p.second.value);
						if (d != nullptr && ImGui::InputDouble("", d))
						{
							c->set_property(p.second.get_name(), *d);
						}
					}
					else if (p.second.type == trigger::property::data_type::Float)
					{
						float* f = std::any_cast<float>(&p.second.value);
						if (f != nullptr && ImGui::InputFloat("", f))
						{
							c->set_property(p.second.get_name(), *f);
						}
					}
					else if (p.second.type == trigger::property::data_type::Int)
					{
						int* i = std::any_cast<int>(&p.second.value);
						if (i != nullptr && ImGui::InputInt("", i))
						{
							c->set_property(p.second.get_name(), *i);
						}
					}
					else if (p.second.type == trigger::property::data_type::String)
					{
						std::string* s = std::any_cast<std::string>(&p.second.value);
						if (s != nullptr && ImGui::InputText("", s))
						{
							c->set_property(p.second.get_name(), *s);
						}
					}
				}
			}
			ImGui::Separator();
		}
	}
	ImGui::End();
}

void trigger::edit::main_editor::draw_console()
{
}

bool trigger::edit::main_editor::new_component()
{
	//Add new Component 
//todo already have??
#ifdef _WIN64
	std::string path("mkdir ..");
	path.append(slash);
	path.append("trigger-component");
	path.append(slash);
	path.append("src");
#else
	std::string path("mkdir trigger-component/src");
#endif

	system(path.c_str());
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
	o << "() : trigger::component(T_CLASS)\n\t{\n\t}\n\nvirtual void update(float delta) noexcept;\nvirtual ~";
	o << this->new_component_name;
	o << "};";
	o.close();
	o.open(new_name.c_str());
	o << "#include \"";
	o << new_component_name;
	o << ".h";
	o << "\"\n";
	o << "void " << this->new_component_name << "::update(float delta) noexcept\n";
	o << "{\n};";
	o << this->new_component_name << "::~" << this->new_component_name << "()\n{\n};";
	o.close();

	//read component cmakelists.txt!
	std::vector<std::string> text;
	std::string cmake;
#ifdef _WIN64
	cmake.append("..");
	cmake.append(slash);
	cmake.append("trigger-component");
	cmake.append(slash);
	cmake.append("CMakeLists.txt");
#else
	cmake.append("trigger-component/CMakeLists.txt");
#endif

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

#ifdef _WIN64
	coms.append("..");
	coms.append(slash);
	coms.append("trigger-component");
	coms.append(slash);
	coms.append("components.h");
#else
	coms.append("trigger-component/components.h");
#endif


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
	std::string path;
#ifdef _WIN64
	path.append("..");
	path.append(slash);
	path.append("trigger-component");
	path.append(slash);
	path.append("src");
	path.append(slash);
	path.append(this->new_component_name);
	path.append(slash);
	path.append(this->new_component_name);
#else
	path.append("trigger-component/src/");
	path.append(this->new_component_name);
	path.append("/");
	path.append(this->new_component_name);
#endif

	if (remove((path + ".h").c_str()) == -1)
	{
		return false;
	}
	remove((path + ".cpp").c_str());

	std::string rm("rmdir ");
#ifdef _WIN64
	rm.append("..");
	rm.append(slash);
	rm.append("trigger-component");
	rm.append(slash);
	rm.append("src");
	rm.append(slash);
	rm.append(this->new_component_name);
#else
	rm.append("trigger-component");
	rm.append(slash);
	rm.append("src");
	rm.append(slash);
	rm.append(this->new_component_name);
#endif
	system( rm.c_str() );

	//read component cmakelists.txt!
	std::list<std::string> text;
	std::string cmake;
#ifdef _WIN64
	cmake.append("..");
	cmake.append(slash);
	cmake.append("trigger-component");
	cmake.append(slash);
	cmake.append("CMakeLists.txt");
#else
	cmake.append("trigger-component/CMakeLists.txt");
#endif

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
#ifdef _WIN64
	coms.append("..");
	coms.append(slash);
	coms.append("trigger-component");
	coms.append(slash);
	coms.append("components.h");
#else
	coms.append("trigger-component/components.h");
#endif
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
