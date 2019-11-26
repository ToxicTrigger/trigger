#include "main_editor.h"
#include <list>
#include <stdlib.h>
#include <fstream>
#include <ctime>
#include "../../tools/macros.h"
#include "../../imgui/misc/cpp/imgui_stdlib.h"
#include "../../../renderer/vk.h"

static bool is_open_editor;

bool trigger::edit::main_editor::draw(VkDevice device, ImGui_ImplVulkanH_Window *wd)
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
		if (ImGui::BeginMenu(this->thread_msg.c_str()))
		{
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Objects"))
		{
			if (ImGui::MenuItem("Create New Object"))
			{
				auto tmp = new trigger::transform();
				this->world->add(tmp);
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
		if (ImGui::Begin("Editor View", &is_open_editor))
		{
			wd->editor_window_pos = ImGui::GetWindowPos();
			wd->editor_window_rect = ImGui::GetWindowSize();
			if (ImGui::IsWindowHovered())
			{
				wd->editor_window_hover = true;
			}
			else
			{
				wd->editor_window_hover = false;
			}
		}
		else
		{
			wd->editor_window_rect = {0,0};
		}
		ImGui::End();
	
		ImGui::Begin("Game View");
		ImGui::End();
	}

	return true;
};

void trigger::edit::main_editor::update(float delta) 
{
	if (thread_run <= 4.0f)
	{
		thread_run += delta * 4;
	}
	else
	{
		thread_run = 0;
	}
	int cur = (int)(thread_run);

	switch (cur)
	{
	case 0:
		this->thread_msg = "-";
		break;
	case 1:
		this->thread_msg = "\\";
		break;
	case 2:
		this->thread_msg = "|";
		break;
	case 3:
		this->thread_msg = "/";
		break;

	}
}

void trigger::edit::main_editor::draw_child(trigger::transform* vec)
{
	if (vec->get_childs().size() != 0 || vec->get_parent() == nullptr)
	{
		if (ImGui::BeginPopupContextWindow())
		{
			if (ImGui::Selectable("Remove This"))
			{
				this->world->delete_object(vec);
				this->current_id = 0;
				this->current_target = nullptr;
				this->current_target_components.clear();
				this->current_name = nullptr;
				ImGui::EndPopup();
				return;
			}
			ImGui::EndPopup();
		}

		if (ImGui::TreeNode(vec->get_name()->c_str()))
		{
			this->current_id = vec->get_instance_id();
			this->current_target = this->world->get_all()[current_id];
			this->current_target_components = this->current_target->get_components();
			this->current_name = this->current_target->get_name();

			for (auto& item : vec->get_childs())
			{
				this->draw_child(item);
			}
			ImGui::TreePop();
		}
	}
}

void trigger::edit::main_editor::draw_objects()
{
	ImGui::Begin("Objects");
	for (auto& obj : this->world->get_objects<trigger::transform>())
	{
		if (obj->get_parent() == nullptr)
		{
			draw_child(obj);

			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
			{
				ImGui::SetDragDropPayload("Transform", obj, sizeof(transform));
				ImGui::EndDragDropSource();
			}

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* data = ImGui::AcceptDragDropPayload("Transform"))
				{
					IM_ASSERT(data->DataSize == sizeof(transform));
					transform tmp = *(const transform*)(data->Data);
					for (auto& target : this->world->get_objects<trigger::transform>())
					{
						if (target->get_instance_id() == tmp.get_instance_id())
						{
							obj->set_parent(target);
						}
					}
				}
				ImGui::EndDragDropTarget();
			}
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
					if (this->current_id != 0)
					{
						auto tmp = trigger::manager::class_manager::get_instance()->get_class_array()->at(this->component_name)->clone();
						tmp->set_instance_id(make_hash_code());
						this->current_target->add_component(tmp);
						this->current_target_components = this->current_target->get_components();
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
						command.append("trigger-component & cmake CMakeLists.txt & cmake --build . --config Debug");
#else
						command.append("cd trigger-component & cmake CMakeLists.txt & cmake --build . --config Debug");
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
		
		ImGui::InputText("", this->current_name);
		for (auto& c : this->current_target_components)
		{
			ImGui::Separator();
			std::string comp_name(c.second->get_type_name());
			ImGui::Text("%s",comp_name.c_str());

			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
			{
				ImGui::SetDragDropPayload("ComponentID", c.second, sizeof(component));
				ImGui::EndDragDropSource();
			}

			if (ImGui::BeginPopupContextWindow())
			{
				if (ImGui::Selectable("Remove This"))
				{
					this->current_target->del_component(c.first);
					ImGui::EndPopup();
					continue;
				}
				ImGui::EndPopup();
			}

			for (auto& p : c.second->properties)
			{
				std::string p_name(p.second.get_name().c_str());
				if (p.second.controllable)
				{
					if (p.second.type == trigger::property::data_type::Bool)
					{
						bool* b = std::any_cast<bool>(&p.second.value);
						if (!p_name.compare("active"))
						{
							ImGui::SameLine();
							if (b != nullptr && ImGui::Checkbox("", b))
							{
								c.second->set_property(p_name, *b);
							}
						}
						else
						{
							if (b != nullptr && ImGui::Checkbox(p_name.c_str(), b))
							{
								c.second->set_property(p.second.get_name(), *b);
							}
						}
					}
					else if (p.second.type == trigger::property::data_type::Double)
					{
						double* d = std::any_cast<double>(&p.second.value);
						if (d != nullptr && ImGui::InputDouble(p_name.c_str(), d))
						{
							c.second->set_property(p.second.get_name(), *d);
						}
						
					}
					else if (p.second.type == trigger::property::data_type::Float)
					{
						float* f = std::any_cast<float>(&p.second.value);
						if (f != nullptr && ImGui::InputFloat(p_name.c_str(), f))
						{
							c.second->set_property(p.second.get_name(), *f);
						}
						
					}
					else if (p.second.type == trigger::property::data_type::Int)
					{
						int* i = std::any_cast<int>(&p.second.value);
						if (i != nullptr && ImGui::InputInt(p_name.c_str(), i))
						{
							c.second->set_property(p.second.get_name(), *i);
						}
						
					}
					else if (p.second.type == trigger::property::data_type::HashID)
					{
						hash_id* i = std::any_cast<hash_id>(&p.second.value);
						if (i != nullptr && ImGui::InputInt(p_name.c_str(), i,1,100,ImGuiInputTextFlags_NoMarkEdited))
						{
							c.second->set_property(p.second.get_name(), *i);
						}

						if (ImGui::BeginDragDropTarget())
						{
							if (const ImGuiPayload* data = ImGui::AcceptDragDropPayload("ComponentID"))
							{
								IM_ASSERT(data->DataSize == sizeof(component));
								component tmp = *(const component*)(data->Data);
								c.second->set_property(p.second.get_name(), tmp.get_instance_id());
							}
							ImGui::EndDragDropTarget();
						}
					}
					else if (p.second.type == trigger::property::data_type::String)
					{
						std::string* s = std::any_cast<std::string>(&p.second.value);
						if (s != nullptr && ImGui::InputText(p_name.c_str(), s))
						{
							c.second->set_property(p.second.get_name(), *s);
						}
						
					}
					else if (p.second.type == trigger::property::data_type::FilePath)
					{
						auto* ss = std::any_cast<trigger::core::file>(&p.second.value);
						if (ss != nullptr && ImGui::InputText(p_name.c_str(), ss->get_path()))
						{
							c.second->set_property(p.second.get_name(), *ss);
						}
						
					}
				}
			}
			c.second->draw_editor();
		}
	}
	ImGui::End();
}

void trigger::edit::main_editor::draw_console()
{
	if (ImGui::Begin("Console",(bool*)nullptr, ImGuiWindowFlags_MenuBar))
	{
		if(ImGui::BeginMenuBar())
		{
			if(ImGui::MenuItem("Clear"))
			{
				trigger::tools::console::get_instance()->clear_log();
			}

			ImGui::Checkbox("Show Log", &trigger::tools::console::get_instance()->view_log);
			ImGui::Checkbox("Show Err", &trigger::tools::console::get_instance()->view_err);
			ImGui::Checkbox("Show Warn", &trigger::tools::console::get_instance()->view_warn);
			if (ImGui::InputText("cmd", &trigger::tools::console::get_instance()->cmd, ImGuiInputTextFlags_EnterReturnsTrue))
			{
				auto cmd = trigger::tools::console::get_instance()->cmd;
				//if cmd == func call
				if (cmd.at(0) == '>')
				{
					auto func = cmd.substr(1, cmd.find(" ")-1);
					auto parm = cmd.substr(cmd.find(" ")+1, cmd.npos);
					if (cmd.find(" ") == cmd.npos)
					{
						parm.clear();
					}
					auto f = trigger::tools::console::get_instance()->funcs.find(func);
					if (f != trigger::tools::console::get_instance()->funcs.end())
					{
						auto tmp = trigger::tools::console::get_instance()->funcs[func].func(parm);
						if(!tmp.empty())
						trigger::tools::console::get_instance()->log(tmp);
					}
					else
					{
						trigger::tools::console::get_instance()->error("Can't Find Func -> " + func);
					}
				}
				else
				{
					trigger::tools::console::get_instance()->log(trigger::tools::console::get_instance()->cmd);
				}
				trigger::tools::console::get_instance()->cmd.clear();
			}
			if (ImGui::IsItemHovered())
			{
				ImGui::SetTooltip("Send Normal Message!\n'>' is call function in trigger::tools::console::get_instance()->funcs\n----------example----------\n >hello 123 or >help");
			}
			ImGui::EndMenuBar();

			ImGui::InputText("Filter", &trigger::tools::console::get_instance()->filter);

		}

		auto logs = trigger::tools::console::get_instance()->get_logs();

		for(unsigned long i = 0; i < logs.size(); ++i)
		{
			auto log = logs[i];
			if (!trigger::tools::console::get_instance()->filter.empty() && log->message.find(trigger::tools::console::get_instance()->filter) == log->message.npos)
			{
				continue;
			}
			
			switch (log->log_type)
			{
				case trigger::tools::log::type::Log:
					if (!trigger::tools::console::get_instance()->view_log) continue;
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{1.f, 1.f,1.f, 1.0f});
					break;
				case trigger::tools::log::type::Error :
					if (!trigger::tools::console::get_instance()->view_err) continue;
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{1.f, 0.2f,0.2f, 1.0f});
					break;
				case trigger::tools::log::type::Warning :
					if (!trigger::tools::console::get_instance()->view_warn) continue;
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{0.7f, 0.5f,0.3f, 1.0f});
					break;
			}

			std::time_t t = std::chrono::system_clock::to_time_t(log->rising_time);

			ImGui::Text("%s", std::ctime(&t));
			ImGui::SameLine();
			if (ImGui::Selectable(log->message.c_str()))
			{
				trigger::tools::console::get_instance()->set_current_log(log);
			}
			ImGui::PopStyleColor();
		}
	}
	ImGui::End();
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
	o << "() : trigger::component(T_CLASS)\n\t{\n\t}\n\n";
	o << "\tvirtual " << this->new_component_name << "* clone() const override\n\t{\n";
	o << "\t\tauto cpy_" << this->new_component_name << " = new " << this->new_component_name << "(*this);\n";
	o << "\t\treturn cpy_" << this->new_component_name << ";\n\t};";
	o << "\n\n\tvirtual void update(float delta) override;\n\tvirtual ~";
	o << this->new_component_name;
	o << "();\n};";
	o.close();
	o.open(new_name.c_str());
	o << "#include \"";
	o << new_component_name;
	o << ".h";
	o << "\"\n";
	o << "void " << this->new_component_name << "::update(float delta)\n";
	o << "{\n};\n";
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
	this->new_component_name = (char*)"";
	return true;
}
;
